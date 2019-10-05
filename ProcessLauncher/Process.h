#pragma once

#include<Windows.h>
#include <iostream>

using namespace std;

class MyProcess {
public:
	int groupKey = 0;
	wstring path = L"";
	wstring params = L"";
	PROCESS_INFORMATION pi;
	DWORD exitCode = 0;
	SYSTEMTIME kTime;
	SYSTEMTIME uTime;

	MyProcess() {}  

	void setup(DWORD &x) {
		exitCode = x;
	}
};
