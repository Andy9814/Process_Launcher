#include<windows.h>
#include<string>
#include<array>
#include<vector>
#include<fstream>
#include <map>
#include<sstream>
#include<iostream>
//#define CreateProcess CreateProcessW;
using namespace std;

// FileReader

void report() {
	cout << "Launch Times" << endl;


}



multimap<int, string> parseFile(ifstream &infile) {
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
				if (isdigit(ch) && charIndex == 1 && !removedFirst)
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
			else if (ch == ',') {
			}
			else {
				foundSpace = true;
			}
		}
	}

	return dataByMultiMap;
}

void terminateApplication( PROCESS_INFORMATION &pi) {
	cout << "Terminate application? (y/n) ";
	char ch = 'n';
	//cin >> ch;
	DWORD exitCode = 0;
	if (ch == 'y')
		::TerminateProcess(pi.hProcess, exitCode);
	else
	{
		if (WAIT_FAILED == WaitForSingleObject(pi.hProcess, INFINITE))
			cerr << "Failure waiting for process to terminate" << endl;

		GetExitCodeProcess(pi.hProcess, &exitCode);
		//cout << GetExitCodeProcess(pi.hProcess, &exitCode) << endl;

	}
	cout << "Process terminated with exit code = " << exitCode << endl;
	CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

/** CreateProcess allows full access to the Win32 OS. */
PROCESS_INFORMATION launch_using_create_process(wstring command) {
	//wstring application = L"\"C:/windows/notepad\"";
	//wstring params = L"";
	//wstring command = application + L" " + params;

	cout << "CreateProcess() launching" << endl;

	STARTUPINFO sinfo = {0};
	sinfo.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi = { 0 };
	unsigned long const CP_MAX_COMMANDLINE = 32768;
	try {
		wchar_t* commandline = new wchar_t[CP_MAX_COMMANDLINE];

		wcsncpy_s(commandline, CP_MAX_COMMANDLINE, command.c_str(), command.size() + 1);
		auto res = CreateProcess(NULL, // application name is null since provided in commandline (doesn't use PATH)
			commandline, // contains application name + params
			NULL,		// same security as parent
			NULL,
			false,
			CREATE_NEW_CONSOLE, // create new console
			NULL,
			NULL, // same current directory as parent
			&sinfo,		// startup options
			&pi		// process information
		);

		if (res == 0) {
			cerr << "Error: " << GetLastError() << endl;
		//	return;
		}

		delete[] commandline;
	}
	catch (std::bad_alloc&) {
		wcerr << L"Insufficient memory to launch application" << endl;
	//	return;
	}


	DWORD exitCode = 0;
	if (WAIT_FAILED == WaitForSingleObject(pi.hProcess, INFINITE))
		cerr << "Failure waiting for process to terminate" << endl;
	GetExitCodeProcess(pi.hProcess, &exitCode);
	//cout << GetExitCodeProcess(pi.hProcess, &exitCode) << endl;
	cout << "Process terminated with exit code = " << exitCode << endl;
	
	return pi;


	//cout << "Process terminated with exit code = " << exitCode << endl;

	////// output times
	//FILETIME creationTime, exitTime, kernelTime, userTime;
	//GetProcessTimes(pi.hProcess, &creationTime, &exitTime, &kernelTime, &userTime);
	//SYSTEMTIME kTime;
	//::FileTimeToSystemTime(&kernelTime, &kTime);
	//cout << "Kernel Time = " << kTime.wHour << ":" << kTime.wMinute << ":" << kTime.wSecond << "." << kTime.wMilliseconds << endl;

	//unsigned long long elapsedTicks = *reinterpret_cast<unsigned long long*>(&exitTime) -
	//	*reinterpret_cast<unsigned long long*>(&creationTime);

	//cout << "Elapsed Ticks = " << elapsedTicks << endl;
	//cout << "Elapsed Time = " << elapsedTicks / 10000000.0 << endl;

	//CloseHandle(pi.hThread);
	//CloseHandle(pi.hProcess);

	//cout << "CreateProcess() landing" << endl;
}

int main(int argc, char* argv[]) {
	multimap <int, string> dataByMultiMap;
	map<int, vector<pair<wstring, wstring>>> MapPair;
	bool checkOut = false;
	// if Argc.size = 1
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
		cout << "File is now open!\nContains:\n";
		dataByMultiMap = parseFile(infile);
	}
	else {
		cout << "ERROR:  File \"" << argv[1] << "\"  does not exist.";
	}
	vector<pair<wstring, wstring>> vec;
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
			PROCESS_INFORMATION pi{0};
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
				vec.push_back(pair(path, params));
				it = dataByMultiMap.erase(it);
				break;
			}
			else {
				MapPair.insert(pair(xt, vec));
				vec.clear();
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
					vec.push_back(pair(path, params));
					it = dataByMultiMap.erase(it);
					break;
				}
			}
		}
	}
	MapPair.insert(pair(xt, vec));
	vec.clear();
	for (auto x : MapPair) {
		wcout << x.first <<endl;
		for (auto xt : x.second) {
			wcout << xt.first << " -- " << xt.second << endl;
		}
		cout << endl;
	}

	return 0;
}
