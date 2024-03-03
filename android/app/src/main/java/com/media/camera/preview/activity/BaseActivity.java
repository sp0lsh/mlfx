package com.media.camera.preview.activity;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.DialogFragment;
import android.support.v4.app.FragmentActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.opengl.GLSurfaceView;
import android.util.DisplayMetrics;
import android.util.Log;
import android.util.Size;
import android.util.SparseIntArray;
import android.view.MotionEvent;
import android.view.Surface;
import android.widget.TextView;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.DialogFragment;
import android.util.Log;
import android.widget.FrameLayout;
import android.widget.Toast;

import com.media.camera.preview.R;
import com.media.camera.preview.capture.PreviewFrameHandler;
import com.media.camera.preview.capture.VideoCameraPreview;
import com.media.camera.preview.render.VideoRenderer;

import java.util.ArrayList;
import java.util.List;

// Execution flow
// VideoCapture.onImageAvailable
// VideoCapture.YUV_420_888_data
// BaseActivity.onPreviewFrame
// BaseActivity.getOrientation
// VideoRenderer.drawVideoFrame
// VideoRenderer.requestRender
// VideoRenderer.onDrawFrame

public class BaseActivity extends FragmentActivity implements PreviewFrameHandler,
        ActivityCompat.OnRequestPermissionsResultCallback {

    public static final String TAG = "CameraGL";

    private static final SparseIntArray ORIENTATIONS = new SparseIntArray();
    protected VideoCameraPreview mPreview;    
    private VideoRenderer mVideoRenderer;
    protected int mParams;

    private static final int REQUEST_CAMERA_PERMISSION = 1;
    private static final String[] CAMERA_PERMISSIONS = {
            Manifest.permission.CAMERA
    };

    static {
        ORIENTATIONS.append(Surface.ROTATION_0, 90);
        ORIENTATIONS.append(Surface.ROTATION_90, 0);
        ORIENTATIONS.append(Surface.ROTATION_180, 270);
        ORIENTATIONS.append(Surface.ROTATION_270, 180);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "BaseActivity.onCreate");

        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);

        mPreview = new VideoCameraPreview(this);
        mPreview.init(displayMetrics.widthPixels, displayMetrics.heightPixels);
        Log.d(TAG, "BaseActivity.onCreate: Resolutions: " + mPreview.getOutputSizes());


        setContentView(R.layout.activity_gl);

        GLSurfaceView glSurfaceView = findViewById(R.id.gl_surface_view);
        mVideoRenderer = new VideoRenderer();
        mVideoRenderer.init(glSurfaceView);

        ((FrameLayout) findViewById(R.id.preview)).addView(mPreview);
    }

    @Override
    public void onPause() {
        Log.d(TAG,"BaseActivity.onPause");
        if (hasPermissionsGranted()) {
            mPreview.stopCamera();
        }
        super.onPause();
    }

    @Override
    public void onResume() {
        super.onResume();
        Log.d(TAG,"BaseActivity.onResume");
        if (!hasPermissionsGranted()) {
            requestPermissions(CAMERA_PERMISSIONS, REQUEST_CAMERA_PERMISSION);
        } else {
            mPreview.startCamera();
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.d(TAG,"BaseActivity.onDestroy");
        mVideoRenderer.destroyRender();
    }

    @Override
    public void onPreviewFrame(byte[] data, int width, int height) {
        Log.d(TAG,"BaseActivity.onPreviewFrame");
        mVideoRenderer.drawVideoFrame(data, width, height, getOrientation());
        mVideoRenderer.requestRender();
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        Log.d(TAG,"BaseActivity.onRequestPermissionsResult");
        if (requestCode == REQUEST_CAMERA_PERMISSION) {
            if (grantResults.length == CAMERA_PERMISSIONS.length) {
                for (int result : grantResults) {
                    if (result != PackageManager.PERMISSION_GRANTED) {

                        Toast.makeText(
                                this,
                                "Failed to grant Camera permission!",
                                Toast.LENGTH_LONG
                            ).show();                        

                    } else {
                        
                        mPreview.startCamera();
                    }
                }
            }
        } else {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        }
    }

    private boolean hasPermissionsGranted() {
        Log.d(TAG,"BaseActivity.hasPermissionsGranted");
        for (String permission : BaseActivity.CAMERA_PERMISSIONS) {
            if (ActivityCompat.checkSelfPermission(this, permission)
                    != PackageManager.PERMISSION_GRANTED) {
                return false;
            }
        }
        return true;
    }

    protected int getOrientation() {
        Log.d(TAG,"BaseActivity.getOrientation");
        int rotation = getWindowManager().getDefaultDisplay().getRotation();
        return (ORIENTATIONS.get(rotation) + mPreview.getSensorOrientation() + 270) % 360;
    }
}
