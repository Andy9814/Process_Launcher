#pragma once
/*
	Nripdeep Singh
	Process Launcher
	OCT 4th,2019
*/
#include<Windows.h>
#include <iostream>

using namespace std;

class MyProcess {
public:
	int groupKey = 0;
	wstring path = L"";
	wstring params = L"";
	PROCESS_INFORMATION pi = { 0 };
	DWORD exitCode = 0;
	SYSTEMTIME kTime = { 0 };
	SYSTEMTIME uTime = { 0 };
	HANDLE handle = nullptr;
	HANDLE hThread = nullptr;
};
