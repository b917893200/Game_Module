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

struct my_point
{
    unsigned x;
    unsigned y;
    my_point() { x = 0; y = 0; }
    my_point(unsigned xx, unsigned yy) :x(xx), y(yy) {}
};

class MouseAndKeyboard
{
public:
	//
	static void click(HWND wnd, const cv::Mat& m1, const cv::Mat& m2);
    static void click(HWND wnd, my_point p);
	//后台鼠标
	static void click(HWND wnd, my_point point, unsigned type);

	static BYTE scan_code(DWORD pKey)
	{
		const DWORD result = MapVirtualKey(pKey, MAPVK_VK_TO_VSC);
		return static_cast<BYTE>(result);
	}
	static void press_key(DWORD pKey)
	{
		//SetForegroundWindow(g.wnd);
		keybd_event(static_cast<BYTE>(pKey), scan_code(pKey), 0, 0);
	}
	static void release_key(DWORD pKey)
	{
		//SetForegroundWindow(g.wnd);
		keybd_event(static_cast<BYTE>(pKey), scan_code(pKey), KEYEVENTF_KEYUP, 0);
	}
	//后台按键
	static  void press_key(HWND wnd,DWORD pKey)
	{
		SendMessageW(wnd, WM_KEYDOWN,pKey,0);
	}
	//后台按键
	static void release_key(HWND wnd,DWORD pKey)
	{
		SendMessageW(wnd, WM_KEYUP, pKey, 0x6000000);
	}
	
};

