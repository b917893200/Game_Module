#include"MouseAndKeyboard.h"
using namespace std;
using namespace cv;
//点击图片的中心
void MouseAndKeyboard::click(HWND wnd, const cv::Mat& m1, const cv::Mat& m2)
{
    cv::Point maxloc;
    cv::Mat result;
    cv::matchTemplate(m1, m2, result, cv::TM_CCOEFF_NORMED);//匹配
    cv::minMaxLoc(result, NULL, NULL, NULL, &maxloc);
    //点击的目的地坐标
    Point p(m2.cols / 2 + maxloc.x, m2.rows / 2 + maxloc.y);
    SetForegroundWindow(wnd);
    Sleep(50);
    SetCursorPos(p.x, p.y);
    cout << "x坐标" << p.x << endl << "y坐标" << p.y << endl;
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    Sleep(50);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}


void MouseAndKeyboard::click(HWND wnd,my_point p)
{
    SetForegroundWindow(wnd);
    Sleep(50);
    SetCursorPos(p.x, p.y);
    //cout << "x坐标" << p.x << endl << "y坐标" << p.y << endl;
    Sleep(50);
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    Sleep(50);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void MouseAndKeyboard::click(HWND wnd, my_point point,unsigned type)
{
    SendMessage(wnd, WM_LBUTTONDOWN, 0, MAKELPARAM(point.x, point.y));
    SendMessage(wnd, WM_LBUTTONUP, 0, MAKELPARAM(point.x, point.y));
}



