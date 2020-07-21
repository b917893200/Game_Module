#include"CaptureGameScreen.h"
using namespace std;
using namespace cv;
long CaptureGameScreen::width;
long CaptureGameScreen::height;

CaptureGameScreen::CaptureGameScreen(HWND GameHwnd)
{
    this->wnd = GameHwnd; 
    MoveWindow(GameHwnd, 0, 0, 800, 600, false);
    hdc = GetDC(GameHwnd);
    RECT rect;
    GetWindowRect(GameHwnd, &rect);
    width = rect.right - rect.left;   //���ڵĿ��
    height = rect.bottom - rect.top;  //���ڵĸ߶�
    //cout << "��" << width << "  " << "��" << height << endl;
}

void CaptureGameScreen::CaptureGameScreenShot(POINT x,POINT y)
{
    //��Щ��Ϸ��̨��ˢ�»��棬��Ҫ���ֽ���
    //SetForegroundWindow(wnd);

    HDC mem_dc;
    HBITMAP hbm_screen;
    BITMAP bmpScreen;
    /*

    ��ĳЩ����£�������ϣ��Ӧ�ó��򲶻�ͼ�񲢽�������ʱ�洢�����磬�������Ż������ڻ�ͼӦ�ó����д�����ͼƬʱ����Ӧ�ó��������ʱ����ͼ�����ͨ��ͼ����ʾ���ŵ���ͼ���Ժ󣬵��û�ѡ����ͨ��ͼʱ��Ӧ�ó�����뽫���ź��ͼ���滻Ϊ����ʱ�������ͨ��ͼ�ĸ�����

    ��Ҫ��ʱ�洢ͼ������Ӧ�ó���������CreateCompatibleDC�������뵱ǰ����DC���ݵ�DC���������ݵ�DC�󣬿���ͨ������CreateCompatibleBitmap���������������ʵ��ߴ��λͼ��Ȼ��ͨ������SelectObject��������ѡ�񵽸��豸�������С�
    */
    try
    {
        mem_dc = ::CreateCompatibleDC(hdc);
        //�������ָ�����豸��������������豸���ݵ�λͼ��
        hbm_screen = ::CreateCompatibleBitmap(
            hdc,
            y.x - x.x,
            y.y - x.y
        );
        //����
       //std::cout << "��" << rect.right - rect.left << std::endl;
       //std::cout << "��" << rect.bottom - rect.top << std::endl;
        SelectObject(mem_dc, hbm_screen);
        BitBlt(mem_dc, 0, 0,
            y.x - x.x,
            y.y - x.y,
            hdc, x.x, x.y,
            SRCCOPY);

        // Retrieve the handle to a display device context for the client
        // area of the window.
        HBITMAP hbmScreen = (HBITMAP)GetCurrentObject(mem_dc, OBJ_BITMAP);

        // Get the BITMAP from the HBITMAP
        GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

        BITMAPFILEHEADER   bmfHeader;
        memset(&bmfHeader, 0, sizeof(BITMAPFILEHEADER));
        BITMAPINFOHEADER   bi;
        memset(&bi, 0, sizeof(BITMAPINFOHEADER));

        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = bmpScreen.bmWidth;
        bi.biHeight = bmpScreen.bmHeight;
        bi.biPlanes = 1;
        bi.biBitCount = 32;
        bi.biCompression = BI_RGB;
        bi.biSizeImage = 0;
        bi.biXPelsPerMeter = 0;
        bi.biYPelsPerMeter = 0;
        bi.biClrUsed = 0;
        bi.biClrImportant = 0;

        DWORD dwBmpSize = GetBitmapBytes(bmpScreen.bmWidth, bmpScreen.bmHeight, 32);
        
        // Add the size of the headers to the size of the bitmap to get the total file size
        DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

        screen_buf = new BYTE[dwBmpSize];
        if (!screen_buf)
            throw runtime_error("buf allocate failed");

        // Gets the "bits" from the bitmap and copies them into a buffer
        // which is pointed to by lpbitmap.
        GetDIBits(hdc, hbmScreen, 0,
            (UINT)bmpScreen.bmHeight,
            screen_buf,
            (BITMAPINFO*)&bi, DIB_RGB_COLORS);

      
        //Offset to where the actual bitmap bits start.
        bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
        bmfHeader.bfReserved1 = bmfHeader.bfReserved2 = 0;
        bmfHeader.bfSize = dwSizeofDIB;
        bmfHeader.bfType = 0x4D42; //BM

        _screen_data.clear();

        _screen_data.reserve(dwSizeofDIB);

        for (int i = 0; i < sizeof(BITMAPFILEHEADER); i++) {
            uchar* buf = (uchar*)&bmfHeader;
            _screen_data.push_back(buf[i]);
        }

        for (int i = 0; i < sizeof(BITMAPINFOHEADER); i++) {
            uchar* buf = (uchar*)&bi;
            _screen_data.push_back(buf[i]);
        }
        //exception
        for (int i = 0; i < dwBmpSize; i++) {
            _screen_data.push_back(screen_buf[i]);
        }

    }
    catch (...) {
        std::cout << "get_screen_data exception";
    }

    DeleteObject(mem_dc);
    DeleteObject(hbm_screen);
    delete []screen_buf;

    //����ͼ��д��mat��
    mat = imdecode(_screen_data, IMREAD_COLOR);
}

int CaptureGameScreen::GetBytesPerPixel(int depth)
{
    return (depth == 32 ? 4 : 3);
}

int CaptureGameScreen::GetBytesPerRow(int width, int depth)
{
    int bytesPerPixel = GetBytesPerPixel(depth);
    int bytesPerRow = ((width * bytesPerPixel + 3) & ~3);
    return bytesPerRow;
}

int CaptureGameScreen::GetBitmapBytes(int width, int height, int depth)
{
    return height * GetBytesPerRow(width, depth);
}

void CaptureGameScreen::show_mat(cv::Mat& m)
{
    cv::namedWindow("123", cv::WINDOW_AUTOSIZE);
    cv::imshow("123", m);
    cv::waitKey(0);
}

//�ж��Ƿ�ʶ��
bool CaptureGameScreen::if_mat_is_matching(cv::Mat& m1, cv::Mat& m2)
{
    cv::Mat result;
    double maxval;
    cv::matchTemplate(m1, m2, result, cv::TM_CCOEFF_NORMED);
    cv::minMaxLoc(result, NULL, &maxval, NULL, NULL);
    if (maxval >= 0.95)
        return true;
    else
    {
        return false;
    }
}

bool CaptureGameScreen::if_mat_is_matching(cv::Mat& m1, cv::Mat& m2, double set)
{
    cv::Mat result;
    double maxval;
    cv::matchTemplate(m1, m2, result, cv::TM_CCOEFF_NORMED);
    cv::minMaxLoc(result, NULL, &maxval, NULL, NULL);
    if (maxval >= set)
        return true;
    else
    {
        return false;
    }

}



