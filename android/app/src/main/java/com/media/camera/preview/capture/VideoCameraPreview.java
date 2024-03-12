package com.media.camera.preview.capture;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.ImageFormat;
import android.graphics.SurfaceTexture;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCaptureSession;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.hardware.camera2.CaptureRequest;
import android.hardware.camera2.params.StreamConfigurationMap;
import android.media.ImageReader;
import android.os.Handler;
import android.os.HandlerThread;
import android.support.annotation.NonNull;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.util.Size;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

public class VideoCameraPreview extends SurfaceView implements SurfaceHolder.Callback {
    public static final String TAG = "CameraGL";

    private final VideoCapture mVideoCapture;
    private final Context mContext;
    private final Semaphore mCameraOpenCloseLock = new Semaphore(1);
    private CameraCaptureSession mCaptureSession;
    private CameraDevice mCameraDevice;
    private String mCameraId;
    private Handler mBackgroundHandler;
    private HandlerThread mBackgroundThread;
    private ImageReader mImageReader;
    private Integer mSensorOrientation;
    private List<Size> mOutputSizes = new ArrayList<>();
    private Size mPreviewSize;

    public VideoCameraPreview(Context context) {
        super(context);

        Log.d(TAG,"VideoCameraPreview.ctor");

        mContext = context;
        mVideoCapture = new VideoCapture((PreviewFrameHandler) context);
        getHolder().addCallback(this);
        setVisibility(View.GONE);
    }

    public void init(int w, int h) {
        Log.d(TAG,"VideoCameraPreview.init");
        mPreviewSize = getOptimalPreviewSize(w, h);
    }

    public void surfaceCreated(SurfaceHolder holder) {
        Log.d(TAG,"VideoCameraPreview.surfaceCreated");
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.d(TAG,"VideoCameraPreview.surfaceDestroyed");
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
        Log.d(TAG,"VideoCameraPreview.surfaceChanged: f:"+ format + ", " + w + "x" + h);
        if (null == mImageReader && null != mPreviewSize) {
            mImageReader = ImageReader.newInstance(mPreviewSize.getWidth(), mPreviewSize.getHeight(), ImageFormat.YUV_420_888, 2);
            mImageReader.setOnImageAvailableListener(mVideoCapture, mBackgroundHandler);
            openCamera();
        }
    }

    public Integer getSensorOrientation() {
        return mSensorOrientation;
    }

    public List<Size> getOutputSizes() {
        return mOutputSizes;
    }

    public void startCamera() {
        Log.d(TAG,"VideoCameraPreview.startCamera");
        startBackgroundThread();
        setVisibility(View.VISIBLE);
    }

    public void stopCamera() {
        Log.d(TAG,"VideoCameraPreview.stopCamera");
        mImageReader.setOnImageAvailableListener(null, null);
        closeCamera();
        stopBackgroundThread();
        setVisibility(View.GONE);
    }

    public void changeSize(Size size) {
        Log.d(TAG,"VideoCameraPreview.changeSize s: " + size);

        mPreviewSize = size;

        stopCamera();
        startCamera();
    }

    public void openCamera() {
        Log.d(TAG,"VideoCameraPreview.openCamera");
        if (ContextCompat.checkSelfPermission(mContext, Manifest.permission.CAMERA)
                != PackageManager.PERMISSION_GRANTED) {
            return;
        }

        Log.d(TAG, "openCamera");

        CameraManager manager = (CameraManager) mContext.getSystemService(Context.CAMERA_SERVICE);
        try {
            if (!mCameraOpenCloseLock.tryAcquire(2500, TimeUnit.MILLISECONDS)) {
                throw new RuntimeException("Time out waiting to lock camera opening.");
            }
            manager.openCamera(mCameraId, mStateCallback, mBackgroundHandler);
        } catch (CameraAccessException e) {
            Log.e(TAG, "Cannot access the camera." + e.toString());
        } catch (InterruptedException e) {
            throw new RuntimeException("Interrupted while trying to lock camera opening.", e);
        }
    }

    public void closeCamera() {
        Log.d(TAG,"VideoCameraPreview.closeCamera");
        try {
            mCameraOpenCloseLock.acquire();
            if (null != mCaptureSession) {
                mCaptureSession.close();
                mCaptureSession = null;
            }
            if (null != mCameraDevice) {
                mCameraDevice.close();
                mCameraDevice = null;
            }
            if (null != mImageReader) {
                mImageReader.close();
                mImageReader = null;
            }
        } catch (InterruptedException e) {
            throw new RuntimeException("Interrupted while trying to lock camera closing.", e);
        } finally {
            mCameraOpenCloseLock.release();
        }

        Log.d(TAG, "closeCamera");
    }

    public void startBackgroundThread() {
        Log.d(TAG,"VideoCameraPreview.startBackgroundThread");
        mBackgroundThread = new HandlerThread("CameraBackground");
        mBackgroundThread.start();
        mBackgroundHandler = new Handler(mBackgroundThread.getLooper());
    }

    public void stopBackgroundThread() {
        Log.d(TAG,"VideoCameraPreview.stopBackgroundThread");
        mBackgroundThread.quitSafely();
        try {
            mBackgroundThread.join();
            mBackgroundThread = null;
            mBackgroundHandler = null;
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    
    private final CameraDevice.StateCallback mStateCallback = new CameraDevice.StateCallback() {

        @Override
        public void onOpened(@NonNull CameraDevice cameraDevice) {
            Log.d(TAG,"VideoCameraPreview.CameraDevice.onOpened");
            // This method is called when the camera is opened.  We start camera preview here.
            mCameraOpenCloseLock.release();
            mCameraDevice = cameraDevice;
         
            // create capture session
            try {
                if (null == mCameraDevice || null == mImageReader) return;
                mCameraDevice.createCaptureSession(Collections.singletonList(mImageReader.getSurface()),
                        sessionStateCallback, mBackgroundHandler);

            } catch (CameraAccessException e) {
                Log.e(TAG, "createCaptureSession " + e.toString());
            }
        }

        @Override
        public void onDisconnected(@NonNull CameraDevice cameraDevice) {
            Log.d(TAG,"VideoCameraPreview.CameraDevice.onDisconnected");
            mCameraOpenCloseLock.release();
            cameraDevice.close();
            mCameraDevice = null;
        }

        @Override
        public void onError(@NonNull CameraDevice cameraDevice, int error) {
            Log.d(TAG,"VideoCameraPreview.CameraDevice.onError");
            mCameraOpenCloseLock.release();
            cameraDevice.close();
            mCameraDevice = null;
        }
    };

    private final CameraCaptureSession.StateCallback sessionStateCallback = new CameraCaptureSession.StateCallback() {
        @Override
        public void onConfigured(@NonNull CameraCaptureSession session) {
            Log.d(TAG,"VideoCameraPreview.CameraCaptureSession.onConfigured");
            mCaptureSession = session;
            try {
                CaptureRequest captureRequest = createCaptureRequest();
                if (captureRequest != null) {
                    session.setRepeatingRequest(captureRequest, null, mBackgroundHandler);
                } else {
                    Log.e(TAG, "captureRequest is null");
                }
            } catch (CameraAccessException e) {
                Log.e(TAG, "onConfigured " + e.toString());
            }
        }

        @Override
        public void onConfigureFailed(@NonNull CameraCaptureSession session) {
            Log.d(TAG,"VideoCameraPreview.CameraCaptureSession.onConfigureFailed");
        }
    };

    private CaptureRequest createCaptureRequest() {
        Log.d(TAG,"VideoCameraPreview.createCaptureRequest");
        if (null == mCameraDevice) return null;
        try {
            CaptureRequest.Builder builder = mCameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_RECORD);
            builder.addTarget(mImageReader.getSurface());
            return builder.build();
        } catch (CameraAccessException e) {
            Log.e(TAG, e.getMessage());
            return null;
        }
    }

    public Size getOptimalPreviewSize(int w, int h) {
        Log.d(TAG,"VideoCameraPreview.getOptimalPreviewSize");
        CameraManager manager = (CameraManager) mContext.getSystemService(Context.CAMERA_SERVICE);
        try {
            for (String cameraId : manager.getCameraIdList()) {
                CameraCharacteristics characteristics = manager.getCameraCharacteristics(cameraId);

                // We don't use a front facing camera in this sample.
                Integer facing = characteristics.get(CameraCharacteristics.LENS_FACING);
                if (facing != null && facing == CameraCharacteristics.LENS_FACING_FRONT) {
                    continue;
                }
                StreamConfigurationMap streamConfigs = characteristics.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);
                if (streamConfigs != null) {
                    mOutputSizes = Arrays.asList(streamConfigs.getOutputSizes(SurfaceTexture.class));
                }
                mSensorOrientation = characteristics.get(CameraCharacteristics.SENSOR_ORIENTATION);
                mCameraId = cameraId;
            }
        } catch (CameraAccessException e) {
            Log.e(TAG, "Cannot access the camera." + e.toString());
        }

        // Use a very small tolerance because we want an exact match.
        final double ASPECT_TOLERANCE = 0.1;
        double targetRatio = (double) w / h;
        if (mOutputSizes == null)
            return null;

        Size optimalSize = null;

        // Start with max value and refine as we iterate over available preview sizes. This is the
        // minimum difference between view and camera height.
        double minDiff = Double.MAX_VALUE;

        // Try to find a preview size that matches aspect ratio and the target view size.
        // Iterate over all available sizes and pick the largest size that can fit in the view and
        // still maintain the aspect ratio.
        for (Size size : mOutputSizes) {
            double ratio = (double) size.getWidth() / size.getHeight();
            if (Math.abs(ratio - targetRatio) > ASPECT_TOLERANCE)
                continue;
            if (Math.abs(size.getHeight() - h) < minDiff) {
                optimalSize = size;
                minDiff = Math.abs(size.getHeight() - h);
            }
        }

        // Cannot find preview size that matches the aspect ratio, ignore the requirement
        if (optimalSize == null) {
            minDiff = Double.MAX_VALUE;
            for (Size size : mOutputSizes) {
                if (Math.abs(size.getHeight() - h) < minDiff) {
                    optimalSize = size;
                    minDiff = Math.abs(size.getHeight() - h);
                }
            }
        }

        return optimalSize;
    }
}
