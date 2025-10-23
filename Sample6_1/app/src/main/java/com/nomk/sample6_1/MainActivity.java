package com.nomk.sample6_1;

import androidx.annotation.NonNull;
import androidx.core.view.WindowInsetsCompat;
import androidx.core.view.WindowInsetsControllerCompat;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;


public class MainActivity extends Activity implements SurfaceHolder.Callback {

    // Used to load the 'sample6_1' library on application startup.
    static {
        System.loadLibrary("sample6_1");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // 1. 配置窗口延伸和状态栏透明（关键：解决顶部留白）
        getWindow().getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE |
                        View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
        );
        getWindow().setStatusBarColor(Color.TRANSPARENT);
        getWindow().setNavigationBarColor(Color.TRANSPARENT); // 导航栏也透明（可选）

        // 2. 处理刘海屏（API 28+）
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            WindowManager.LayoutParams lp = getWindow().getAttributes();
            lp.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            getWindow().setAttributes(lp);
        }

        // 3. 隐藏状态栏和导航栏（使用 WindowInsetsControllerCompat）
        WindowInsetsControllerCompat insetsController = new WindowInsetsControllerCompat(getWindow(), getWindow().getDecorView());
        insetsController.hide(WindowInsetsCompat.Type.navigationBars() | WindowInsetsCompat.Type.statusBars());
        insetsController.setSystemBarsBehavior(WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);


        // 显示Vulkan状态
//        TextView tv = findViewById(R.id.sample_text);
//        tv.setText(stringFromJNI());

        // 初始化SurfaceView用于Vulkan渲染
        SurfaceView surfaceView = findViewById(R.id.surfaceView);
        SurfaceHolder holder = surfaceView.getHolder();
        holder.addCallback(this);

        AssetManager assetManager = getAssets();
        setAssetManager(assetManager);
    }

    /**
     * A native method that is implemented by the 'sample6_1' native library,
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