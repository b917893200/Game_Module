#include"cDrvCtrl.h"
#include<string>
void cDrvCtrl::install(PCHAR m_pSysPath)
{
	if (!IsRunasAdmin()) {
		MessageBoxA(NULL, "����Ա������У�", "��ʾ", MB_OK);
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
		std::cout << "��װ�ɹ�!" << std::endl;
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
        p_name, //�����������ע����е�����
		p_name, // ע������������ DisplayName ֵ
        SERVICE_ALL_ACCESS, // ������������ķ���Ȩ��
        SERVICE_KERNEL_DRIVER,// ��ʾ���صķ�������������
        SERVICE_DEMAND_START, // ע������������ Start ֵ
        SERVICE_ERROR_IGNORE, // ע������������ ErrorControl ֵ
        m_pSysPath, // ע������������ ImagePath ֵ
        NULL,
        NULL,
        NULL,
        NULL,
        NULL);
	if (m_hService == NULL) {
		m_dwLastError = GetLastError();
		if (m_dwLastError != ERROR_IO_PENDING && m_dwLastError != ERROR_SERVICE_EXISTS)
		{
			//��������ԭ�򴴽�����ʧ��
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
				//����򿪷���Ҳʧ�ܣ�����ζ����
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
					//�豸����ס
					bRet = FALSE;
					return bRet;
				}
				else
				{
#ifdef  _d
					std::cout << __FILE__ << " " << __LINE__ << std::endl;
					std::cout << "last error is " << m_dwLastError << std::endl;
#endif 
					//�����Ѿ�����
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