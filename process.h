#include<Windows.h>
#include<TlHelp32.h>
#define _d 
#ifdef _d
#include<iostream>
#endif 

class process
{
	//进程ID
	DWORD ID = 0;
	void* modBaseAddr = nullptr;
public:
	process() = delete;
	~process() {
		
 	}
	//传入一个进程名
	process(const wchar_t* ProcessName);
	//传入一个进程ID
	process(DWORD ID) { this->ID = ID; }
	void GetProcessBaseAddr();
	void* RetBaseAddr() { GetProcessBaseAddr(); if (modBaseAddr) return modBaseAddr; }
	DWORD RetId() { if (ID) return ID; }
};