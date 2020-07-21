#include"MouseAndKeyboard.h"
using namespace std;
using namespace cv;
//���ͼƬ������
void MouseAndKeyboard::click(HWND wnd, const cv::Mat& m1, const cv::Mat& m2)
{
    cv::Point maxloc;
    cv::Mat result;
    cv::matchTemplate(m1, m2, result, cv::TM_CCOEFF_NORMED);//ƥ��
    cv::minMaxLoc(result, NULL, NULL, NULL, &maxloc);
    //�����Ŀ�ĵ�����
    Point p(m2.cols / 2 + maxloc.x, m2.rows / 2 + maxloc.y);
    SetForegroundWindow(wnd);
    Sleep(50);
    SetCursorPos(p.x, p.y);
    cout << "x����" << p.x << endl << "y����" << p.y << endl;
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    Sleep(50);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}


void MouseAndKeyboard::click(HWND wnd,my_point p)
{
    SetForegroundWindow(wnd);
    Sleep(50);
    SetCursorPos(p.x, p.y);
    //cout << "x����" << p.x << endl << "y����" << p.y << endl;
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



