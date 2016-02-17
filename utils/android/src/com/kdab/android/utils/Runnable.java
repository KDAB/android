package com.kdab.android.utils;

class Runnable implements java.lang.Runnable
{
    @Override
    public void run() {
        runPendingCppRunnables();
    }

    public static native void runPendingCppRunnables();
}
