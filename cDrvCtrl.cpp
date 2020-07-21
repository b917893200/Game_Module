#include"cDrvCtrl.h"
#include<string>
void cDrvCtrl::install(PCHAR m_pSysPath)
{
	if (!IsRunasAdmin()) {
		MessageBoxA(NULL, "管理员身份运行！", "提示", MB_OK);
		exit(0);
	}
	this->m_pSysPath = m_pSysPath;
	std::string path(m_pSysPath);
	size_t pos = path.find_last_of('\\');
	pos += 1;
	std::string t = path.substr(pos);
	p_name = new char[t.size() + 1];
	memset(p_name, t.size() + 1, 0);
	memcpy(p_name, t.c_str(), t.size());
#ifdef  _d
	//std::cout << p_name << std::endl;
#endif //  _d

#ifdef _d
	if (start())
		std::cout << "安装成功!" << std::endl;
#elif 
	start();
#endif
}
bool cDrvCtrl::start()
{
	bool bRet;
	m_hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!m_hSCManager) {
		m_dwLastError = GetLastError();
#ifdef  _d
		std::cout << __FILE__ << " " << __LINE__ << std::endl;
		std::cout << m_dwLastError << std::endl;
#endif 
		return false;
	}
	m_hService = CreateServiceA(m_hSCManager,
        p_name, //驱动程序的在注册表中的名字
		p_name, // 注册表驱动程序的 DisplayName 值
        SERVICE_ALL_ACCESS, // 加载驱动程序的访问权限
        SERVICE_KERNEL_DRIVER,// 表示加载的服务是驱动程序
        SERVICE_DEMAND_START, // 注册表驱动程序的 Start 值
        SERVICE_ERROR_IGNORE, // 注册表驱动程序的 ErrorControl 值
        m_pSysPath, // 注册表驱动程序的 ImagePath 值
        NULL,
        NULL,
        NULL,
        NULL,
        NULL);
	if (m_hService == NULL) {
		m_dwLastError = GetLastError();
		if (m_dwLastError != ERROR_IO_PENDING && m_dwLastError != ERROR_SERVICE_EXISTS)
		{
			//由于其他原因创建服务失败
#ifdef  _d
			std::cout << __FILE__ << " " << __LINE__ << std::endl;
			std::cout << "last error is "<< m_dwLastError << std::endl;
#endif 
			return false;
		}
		else
		{
			m_hService = OpenServiceA(m_hSCManager, p_name, SERVICE_ALL_ACCESS);
			if (m_hService == NULL)
			{
				//如果打开服务也失败，则意味错误
				m_dwLastError = GetLastError();
#ifdef  _d
				std::cout << __FILE__ << " " << __LINE__ << std::endl;
				std::cout << "last error is " << m_dwLastError << std::endl;
#endif 
				
				return false;
			}
		}
	}
		bRet = StartServiceA(m_hService, NULL, NULL);
		if (!bRet)
		{
			m_dwLastError = GetLastError();
			if (m_dwLastError != ERROR_IO_PENDING && m_dwLastError != ERROR_SERVICE_ALREADY_RUNNING)
			{
#ifdef  _d
				std::cout << __FILE__ << " " << __LINE__ << std::endl;
				std::cout << "last error is " << m_dwLastError << std::endl;
#endif 
				bRet = FALSE;
				return bRet;
			}
			else
			{
				if (m_dwLastError == ERROR_IO_PENDING)
				{
#ifdef  _d
					std::cout << __FILE__ << " " << __LINE__ << std::endl;
					std::cout << "last error is " << m_dwLastError << std::endl;
#endif 
					//设备被挂住
					bRet = FALSE;
					return bRet;
				}
				else
				{
#ifdef  _d
					std::cout << __FILE__ << " " << __LINE__ << std::endl;
					std::cout << "last error is " << m_dwLastError << std::endl;
#endif 
					//服务已经开启
					bRet = TRUE;
					return bRet;
				}
			}
		}
	bRet = TRUE;
	return bRet;
}

bool cDrvCtrl::IsRunasAdmin() {
	BOOL bElevated = FALSE;
	HANDLE hToken = NULL;

	// Get current process token
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		return FALSE;

	TOKEN_ELEVATION tokenEle;
	DWORD dwRetLen = 0;

	// Retrieve token elevation information
	if (GetTokenInformation(hToken, TokenElevation, &tokenEle, sizeof(tokenEle), &dwRetLen))
	{
		if (dwRetLen == sizeof(tokenEle))
		{
			bElevated = tokenEle.TokenIsElevated;
		}
	}

	CloseHandle(hToken);
	return bElevated;
}