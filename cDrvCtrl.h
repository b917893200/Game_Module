#include<Windows.h>
//功能失败，此宏用来开启调试输出 \
vs 平台可正常使用， 其他平台请关闭
#define _d 
#ifdef _d
#include<iostream>
#endif 
//用来加载驱动
class cDrvCtrl
{

	DWORD m_dwLastError; //最后的错误
	PCHAR m_pSysPath; //驱动路径
	HANDLE m_hDriver; //驱动句柄
	SC_HANDLE m_hSCManager; //SCM句柄
	SC_HANDLE m_hService; //服务句柄
	PCHAR p_name;

public:
	cDrvCtrl()//初始化各个变量
	{
		m_dwLastError = 0;
		m_hSCManager = nullptr;
		m_hService = nullptr;
		m_pSysPath = nullptr;
		p_name = nullptr;
		m_hDriver = INVALID_HANDLE_VALUE;
	}
	~cDrvCtrl()//清除垃圾
	{
		CloseServiceHandle(m_hService);
		CloseServiceHandle(m_hSCManager);
		CloseHandle(m_hDriver);
		delete [] p_name;
	}

public:
	void install(PCHAR m_pSysPath);//传一个驱动路径
	bool IsRunasAdmin();
	bool start();
	unsigned long ret_error() {
		return m_dwLastError;
	}
	bool stop() {
		SERVICE_STATUS ss;
		//程序关闭后句柄为0，但是驱动已经加载
		if (!m_hService) {
			m_hService = OpenServiceA(m_hSCManager, p_name, SERVICE_ALL_ACCESS);
			if (m_hService == NULL)
			{
				m_dwLastError = GetLastError();
				return false;
			}
		}
		if (!ControlService(m_hService, SERVICE_CONTROL_STOP, &ss))
		{
			m_dwLastError = GetLastError();
			return FALSE;
		}
		if (!remove())
			return FALSE;
		return TRUE;
	}
	bool remove() {
		if (!DeleteService(m_hService))
		{
			m_dwLastError = GetLastError();
			return FALSE;
		}
		return TRUE;
	}
};
