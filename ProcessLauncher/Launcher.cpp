/*
	Nripdeep Singh
	Process Launcher
	OCT 4th,2019
*/
#include<windows.h>
#include<string>
#include<vector>
#include<fstream>
#include <map>
#include<iostream>
#include"Process.h"
#include <iomanip>    
using namespace std;

/*
	parseFile read the File and insert each line into a multimap
	which has groupkey as key and path + params as value
*/
multimap<int, string> parseFile(ifstream& infile) {
	string line = "";
	multimap<int, string> dataByMultiMap;
	while (getline(infile, line)) {
		int groupKey = 0;
		int charIndex = 0;
		int groupNum = 0;
		bool foundSpace = false;
		bool removedFirst = false;
		for (char ch : line)
		{
			charIndex++;
			if (ch != ',' && !isspace(ch)) {
				groupKey = ch - 48;
				if (isdigit(ch) && charIndex == 1 && !removedFirst) // if its the first key
				{
					groupNum = groupKey;
					line.erase(line.begin() + 0);
					dataByMultiMap.insert(pair(groupKey, line));
					removedFirst = true;
				}
				if (isdigit(ch) && foundSpace && !removedFirst) {
					groupNum = groupKey;
					line.erase(line.begin(), line.begin() + charIndex);
					dataByMultiMap.insert(pair(groupKey, line));
					removedFirst = true;
				}
			}
			else if (ch == ',') {} // dont do anything if its comma
			else {
				foundSpace = true;
			}
		}
	}
	return dataByMultiMap;
}


/**
	CreateProcess allows to run the process.
*/
void launch_using_create_process(map<int, vector<MyProcess*>> data)
{
	wcout << "Launch Times \n" << endl;
	for (auto groups : data)//group
	{
		for (auto groupData : groups.second)// MyProcess
		{
			wstring command = groupData->path + L" " + groupData->params;
			STARTUPINFO sinfo = { 0 };
			sinfo.cb = sizeof(STARTUPINFO);
			PROCESS_INFORMATION pi = { 0 };
			unsigned long const CP_MAX_COMMANDLINE = 32768;
			try {
				wchar_t* commandline = new wchar_t[CP_MAX_COMMANDLINE];
				wcsncpy_s(commandline, CP_MAX_COMMANDLINE, command.c_str(), command.size() + 1);
				auto res = CreateProcess(NULL,
					commandline,
					NULL,
					NULL,
					false,
					CREATE_NEW_CONSOLE,
					NULL,
					NULL,
					&sinfo,
					&pi
				);
				if (res == 0) {
					wcerr << "Error Code: \"" << GetLastError()<<"\"" << endl;
					wcerr << "Failed to Launch Process: \"" << groupData->path << "\" " << groupData->params << endl;
					exit(EXIT_FAILURE);
				}
				delete[] commandline;
			}
			catch (std::bad_alloc&) {
				wcerr << "Error Code: \"" << GetLastError() << "\"" << endl;
				wcerr << "Failed to Launch Process: \"" << groupData->path << "\" " << groupData->params << endl;
				wcerr << L"Unknown exception: terminating" << endl;
				exit(EXIT_FAILURE);
			}
			// Assign values to the myProcess
			groupData->handle = pi.hProcess; 
			groupData->hThread = pi.hThread;
		}

		// once the group is finished, now time to clean up and close.
		int i = 0;
		for (auto groupDatas : groups.second)
		{
			DWORD exitCode;
			if (WAIT_FAILED == WaitForSingleObject(groupDatas->handle, INFINITE))
				cerr << "Failure waiting for process to terminate" << endl;
			GetExitCodeProcess(groupDatas->handle, &exitCode);
			groupDatas->exitCode = exitCode;
			FILETIME creationTime, exitTime, kernelTime, userTime;
			GetProcessTimes(groupDatas->handle, &creationTime, &exitTime, &kernelTime, &userTime);
			SYSTEMTIME kTime, uTime;
			::FileTimeToSystemTime(&kernelTime, &kTime);
			::FileTimeToSystemTime(&userTime, &uTime);
			groupDatas->kTime = kTime;
			groupDatas->uTime = uTime;
			if (i == 0)
				wcout << "Group:" << groupDatas->groupKey << endl;
			wcout << "K:" << groupDatas->kTime.wHour << ":"
				<< groupDatas->kTime.wMinute << ":"
				<< groupDatas->kTime.wSecond << ":"
				<< groupDatas->kTime.wMilliseconds
				<< setw(3)
				<< "U:" << groupDatas->uTime.wHour << ":"
				<< groupDatas->uTime.wMinute << ":"
				<< groupDatas->uTime.wSecond << ":"
				<< groupDatas->uTime.wMilliseconds
				<< setw(3)
				<< " E:" << groupDatas->exitCode
				<< setw(3)
				<< " G:" << groupDatas->groupKey << " "
				<< groupDatas->path << " "
				<< groupDatas->params << endl;
			i++;
			CloseHandle(groupDatas->handle);
			CloseHandle(groupDatas->hThread);
		}
		cout << endl;
	}
}

/*
	errorReport creates a Report for the error that has occured in the application 
*/
void errorReport(map<int, vector<MyProcess*>> data) {
	cout << "\nErrors" << endl;
	for (auto groupData : data)
		for (auto dd : groupData.second)
			if (dd->exitCode != 0) {
				wcout << "G#: " << dd->groupKey << " Command: " << dd->path << " Params : " << dd->params << endl;
				wcout << " ---> Error = " << dd->exitCode << endl;
			}
}

int main(int argc, char* argv[]) {
	cout << "Launch Processer, (c) Nripdeep Singh\n" << endl;
	// Creating multimap and a map for the values.
	multimap <int, string> dataByMultiMap;
	map<int, vector<MyProcess*>> processessmap;

	bool checkOut = false;
	if (argc > 1) {
		int i = 0;
		while (argv[i]) {
			if (i == argc) {
				break;
			}
			i++;
		}
		if (argc > 2) {
			cout << "more than one paramter!! I am gonna take 1st one." <<
				"[ " << argv[1] << " ]" << endl;
			cout << "*****In Case you are trying to open any path, its could" <<
				" have spaces in between.\n     Please make sure to put the path in double quotes."
				<< endl;
		}
	}
	else {
		cout << "No file name entered. Exiting...";
		return -1;
	}

	// 1  Read a File
	ifstream infile(argv[1]); //open the file
	if (infile.is_open() && infile.good()) {
		dataByMultiMap = parseFile(infile);
	}
	else {
		cout << "ERROR:  File \"" << argv[1] << "\"  does not exist.";
	}
	vector<MyProcess*> vecProcess;
	int xt = 0;
	for (auto it = dataByMultiMap.begin(); it != dataByMultiMap.end(); )
	{
		if (xt == 0)
			xt = (*it).first;
		for (auto c : dataByMultiMap) {
			wstring path;
			wstring params = L"";
			wstring timming = L"";
			wstring temp;
			bool spacess = false;
			if (c.first == xt) {
				for (char x : (*it).second) {

					if (x == ',') {
						if (path == L"")
							path = temp;
						temp = L"";
					}
					else if (isspace(x)) {
						if (path != L"" && temp == L"" && !spacess) {
							spacess = true;
						}
						else if (spacess && path != L"") {
							temp += x;
						}
						else if (temp != L"" && path != L"") {
							temp += x;
						}
					}
					else {
						temp += x;
					}
				}
				if (path != L"" && params == L"") {
					params = temp;
				}
				wstring command = path + L" " + params;
				// Adding the class
				MyProcess* p = new MyProcess();
				p->groupKey = xt;
				p->params = params;
				p->path = path;
				vecProcess.push_back(p);
				it = dataByMultiMap.erase(it); // delete it from the multimap, so it doesnt repeat in the next interation
				break;
			}
			else {
				processessmap.insert(pair(xt, vecProcess));
				vecProcess.clear();
				if (c.first == (*it).first) {
					xt = c.first;
					for (char x : (*it).second) {

						if (x == ',') {
							if (path == L"")
								path = temp;

							temp = L"";
						}
						else if (isspace(x)) {
							if (path != L"" && temp == L"" && !spacess) {
								spacess = true;
							}
							else if (spacess && path != L"") {
								temp += x;
							}
							else if (temp != L"" && path != L"") {
								temp += x;
							}

						}
						else {
							temp += x;
						}
					}
					if (path != L"" && params == L"") {
						params = temp;
					}
					// Adding the class
					MyProcess* p = new MyProcess();
					p->groupKey = xt;
					p->params = params;
					p->path = path;
					vecProcess.push_back(p);
					it = dataByMultiMap.erase(it);
					break;
				}
			}
		}
	}
	processessmap.insert(pair(xt, vecProcess)); // insert the last one
	vecProcess.clear();
	launch_using_create_process(processessmap); 
	errorReport(processessmap);
	return 0;
}
