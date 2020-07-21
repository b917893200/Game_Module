#include"process.h"
using namespace std;
process::process(const wchar_t* ProcessName)
{
	HANDLE  hSnap = NULL; 
	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (!hSnap) {
#ifdef  _d
	std::cout << "CreateToolhelp32Snapshot faild" << std::endl;
#endif //  _d
		return;
	}
	PROCESSENTRY32W  pe = { 0 };
	pe.dwSize = sizeof(PROCESSENTRY32W);
	if (!Process32FirstW(hSnap, &pe))
	{
#ifdef  _d
		std::cout << "Process32FirstW faild" << std::endl;
#endif //  _d
		CloseHandle(hSnap);
	}
	do
	{
		if (!wcscmp(ProcessName, pe.szExeFile))
		{
			this->ID = pe.th32ProcessID;
			break;
		}

	} while (Process32Next(hSnap, &pe));
	CloseHandle(hSnap);
}

void process::GetProcessBaseAddr()
{
	DWORD addressOfChange;//这个因为会变，所有遍历内存中的模块
	HANDLE phSnapshot;
	MODULEENTRY32 me32;//存放快照信息的结构体
	me32.dwSize = sizeof(MODULEENTRY32);
	phSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ID);//创建进程快照
	if (phSnapshot == INVALID_HANDLE_VALUE)
	{
#ifdef  _d
		std::cout << "CreateToolhelp32Snapshot faild" << std::endl;
#endif //  _d
		return;
	}
	if (!Module32First(phSnapshot, &me32))
	{
#ifdef  _d
		std::cout << "Module32First faild" << std::endl;
#endif //  _d
		return;
	}
	do
	{
		if (me32.th32ProcessID == ID)
		{
			this->modBaseAddr = (void*)me32.modBaseAddr;
			break;
		}
	} while (Module32Next(phSnapshot, &me32));
}
