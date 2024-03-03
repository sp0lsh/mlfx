package com.media.camera.preview.render;

import android.view.Surface;


import android.opengl.GLSurfaceView;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


public class VideoRenderer implements GLSurfaceView.Renderer{

    public static final String TAG = "CameraGL";

    private GLSurfaceView mGLSurface;

    public VideoRenderer() {
        create(0);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        Log.d(TAG,"VideoRenderer.onDrawFrame");
        render();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        Log.d(TAG,"VideoRenderer.onSurfaceChanged");
        init(null, width, height);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.d(TAG,"VideoRenderer.onSurfaceCreated");
    }
    
    public void init(GLSurfaceView glSurface) {
        Log.d(TAG,"VideoRenderer.init");

        mGLSurface = glSurface;
        // Create an OpenGL ES 2 context.
        mGLSurface.setEGLContextClientVersion(3);
        mGLSurface.setRenderer(this);
        mGLSurface.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }

    public void destroyRender() {
        Log.d(TAG,"VideoRenderer.destroyRender");
        destroy();
    }

    public void requestRender() {
        Log.d(TAG,"VideoRenderer.requestRender");

        if (mGLSurface != null) {
            mGLSurface.requestRender();
        }
    }

    public void drawVideoFrame(byte[] data, int width, int height, int rotation) {
        Log.d(TAG,"VideoRenderer.drawVideoFrame");
        draw(data, width, height, rotation);
    }

    private long mNativeContext; // using by native

    protected native void create(int type);

    protected native void destroy();

    protected native void init(Surface surface, int width, int height);

    protected native void render();

    protected native void draw(byte[] data, int width, int height, int rotation);

    static {
        System.loadLibrary("media-lib");
    }
}
