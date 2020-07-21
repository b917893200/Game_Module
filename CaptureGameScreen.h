#pragma once
#include<Windows.h>
#include<vector>
#include<iostream>
#include<opencv2/opencv.hpp>
#include<exception>
#include<wingdi.h>
#include<mutex>
#include<chrono>
#include<exception>

class CaptureGameScreen
{
    HDC hdc;
    BYTE* screen_buf = nullptr;
    std::vector<uchar> _screen_data;
    HWND wnd;
    cv::Mat mat;
    static long width;
    static long height;
public:
    //不允许未绑定
    CaptureGameScreen() = delete;
    //绑定到一个游戏窗口上
    CaptureGameScreen(HWND GameHwnd);
    ~CaptureGameScreen() { ReleaseDC(wnd, hdc); };
    void CaptureGameScreenShot(POINT x = { 0,0 }, POINT y = {width,height});
    int GetBytesPerPixel(int depth);
    int GetBytesPerRow(int width, int depth);
    int GetBitmapBytes(int width, int height, int depth);
    void show_mat(cv::Mat& m);
    bool if_mat_is_matching(cv::Mat& m1, cv::Mat& m2, double set);
    bool if_mat_is_matching(cv::Mat& m1, cv::Mat& m2);
    cv::Mat& RetMat() { return mat; };
    HWND& RetHwnd() { return wnd; };
    HDC& RetHdc() { return hdc; };
};