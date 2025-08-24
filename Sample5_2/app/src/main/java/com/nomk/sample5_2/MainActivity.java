package com.nomk.sample5_2;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.TextView;

import com.nomk.sample5_2.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback{

    // Used to load the 'sample5_2' library on application startup.
    static {
        System.loadLibrary("sample5_2");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // 显示Vulkan状态
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        // 初始化SurfaceView用于Vulkan渲染
        SurfaceView surfaceView = findViewById(R.id.surfaceView);
        SurfaceHolder holder = surfaceView.getHolder();
        holder.addCallback(this);

        AssetManager assetManager = getAssets();
        setAssetManager(assetManager);
    }

    /**
     * A native method that is implemented by the 'sample5_2' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void initVulkan(Object surface);

    public native void setAssetManager(AssetManager assetManager);

    public native boolean handleInputEvent(int action, int x, int y, int pointerCount);

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int action = event.getAction();
        int x = (int) event.getX();
        int y = (int) event.getY();
        int pointerCount = event.getPointerCount();
        return handleInputEvent(action, x, y, pointerCount);
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder surfaceHolder, int i, int i1, int i2) {
        initVulkan(surfaceHolder.getSurface());
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder surfaceHolder) {

    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder surfaceHolder) {

    }
}