#include<Windows.h>
#include<TlHelp32.h>
#define _d 
#ifdef _d
#include<iostream>
#endif 

class process
{
	//����ID
	DWORD ID = 0;
	void* modBaseAddr = nullptr;
public:
	process() = delete;
	~process() {
		
 	}
	//����һ��������
	process(const wchar_t* ProcessName);
	//����һ������ID
	process(DWORD ID) { this->ID = ID; }
	void GetProcessBaseAddr();
	void* RetBaseAddr() { GetProcessBaseAddr(); if (modBaseAddr) return modBaseAddr; }
	DWORD RetId() { if (ID) return ID; }
};