#include<windows.h>
#include<iostream>
#include<string>
#include<array>
#include<vector>
#include<fstream>
#include <map>
#include<sstream>
using namespace std;

// FileReader

// Function to remove all spaces from a given string 
string removeSpaces(string str)
{
	str.erase(remove(str.begin(), str.end(), ' '), str.end());
	return str;
}
int main(int argc, char* argv[]) {
	// Do the Temp code
	vector<string> tmpArgs{"1","mini","200","/n",
						   "1","Sanndy","207","/n",
						   "2","Bandy","2400","/n",
						  };
	vector < pair<int, string> >dataByVector;
	multimap <int, string> dataByMultiMap;
	tuple <string, int, int> dataByTuple;
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
			"[ " <<argv[1] <<" ]"<< endl;
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
		string line = "";
		
		while (getline(infile, line)) {
			cout << "line ==== " << line << endl;
			int groupKey = 0;
			char ch;
			//line = removeSpaces(line);
			stringstream ss(line);
			int charIndex = 0;
			int groupNum = 0;
			bool foundSpace = false;
			bool removedFirst = false;
			string storageName; 

		/*	while (ss >> storageName) {
				cout << storageName << endl;
			}*/


			for(char ch : line)
			{
				charIndex++;
					if (ch != ',' && !isspace(ch) ) {
						groupKey = ch - 48;
						if (isdigit(ch) && charIndex ==1 && !removedFirst )
						{
							groupNum = groupKey;
							//line.erase(std::remove(line.begin(), line.end(), ch), line.end());
							line.erase(line.begin() + 0);
							dataByMultiMap.insert(pair(groupKey, line));
							removedFirst = true;
						}
						if (isdigit(ch) && foundSpace && !removedFirst) {
							groupNum = groupKey;
							//line.erase(std::remove(line.begin(), line.end(), ch), line.end());
							line.erase(line.begin(), line.begin() + charIndex );
							dataByMultiMap.insert(pair(groupKey, line));
							removedFirst = true;

						}

						storageName += ch;
						
					}
					else if(ch == ',') {
						get<0>(dataByTuple) = storageName;
						storageName = "";
					
					}
					else  {
						foundSpace = true;
					}
				
					if (ch == '\n')
						break;



			}
		}
		
	}
	else {
		cout << "ERROR:  File \"" << argv[1] <<  "\"  does not exist.";
	}

		// could hit break; once all good
	//cout << "Data By map" << endl;
	//for (auto c : dataByLine) {
	//	cout << c.first << "    -->   " << c.second << endl;;
	//}

	// dont need for llop for this

	cout << "Data By multimap" << endl;
	
	int xt = 0;
		for (auto it = dataByMultiMap.begin(); it != dataByMultiMap.end(); )
		{
			if (xt == 0)
				xt = (*it).first;
		//	cout << (*it).first << (*it).second<< endl;




			for (auto c : dataByMultiMap) {
				if (c.first == xt) {
					cout << "create_process() launching for group" << xt << endl;


		// loop thorugh my values
		string path;
		bool commaSwitch = false;
		bool pathGood = false;
		string errorCode = "";
		string timming = "";
		string temp = "";
		for (char x : (*it).second) {

			if (x == ',') {
				if (path == "") {
					path = temp;
				}
				/*else  if (errorCode == "") {
					errorCode = temp;
				}
				else if (timming == "") {
					timming = temp;
				}*/
				temp = "";


			}
			else if(isspace(x)){
				if (path != "" &&errorCode == "") {
					errorCode = temp;
				}
				
				temp = "";

			}
			else {
				temp += x;

			}
		}
		// not sure about it yet
				if (errorCode != "" && timming == "") {
					timming = temp;
				}







					it = dataByMultiMap.erase(it);
					break;
				}
				else {
					//xt = 0;
					cout << "Ending_Process()  for group" << xt << endl;
					// end of create process
					if (c.first == (*it).first) {
						xt = c.first;

						// apply the same logic now
// createProcess	
						cout << "create_process() launching for group" << xt << endl;


						it = dataByMultiMap.erase(it);
						break;

					}
				}
			}



		




	//auto it = dataByMultiMap.begin();
	/*while(it != dataByMultiMap.end())
	{
			cout << (*it).first << endl;
	//*/	//for (auto x : dataByMultiMap) 
	//	{
	//		if ((*it).first == x.first) {
	//			cout << "create_process() launching for group"<< (*it).first << endl;
	//			// loop thorugh my values
	//			string path;
	//			bool commaSwitch = false;
	//			bool pathGood = false;
	//			string errorCode = "";
	//			string timming = "";
	//			string temp = "";
	//			for (char x : x.second) {
	//			
	//				if (x == ',') {
	//					if (path == "") {
	//						path = temp;
	//					}
	//					else  if(errorCode == ""){
	//						errorCode = temp;
	//					}
	//					else if (timming == "") {
	//						timming = temp;
	//					}
	//					temp = "";


	//				}
	//				else {
	//					temp = x;
	//				}
	//			}




	//			wstring application = L"\"c:/windows/notepad\""; // ShellError
	//			wstring params = L"C:/setup.log";  // 50 2000
	//			wstring command = application + L" " + params;

				//STARTUPINFO sinfo = { 0 };
				//sinfo.cb = sizeof(STARTUPINFO);
				//PROCESS_INFORMATION pi = { 0 };
				//std::uint32_t const CP_MAX_COMMANDLINE = 32768;
				//try {

				//	// this one creates a realy big memory block to compensate if the lenght of the command increases
				//	wchar_t* commandline = new wchar_t[CP_MAX_COMMANDLINE];
				//	wcsncpy_s(commandline, CP_MAX_COMMANDLINE, command.c_str(), command.size() + 1);   // wcscpy wide cstr number copy safe version// hope that the thing you are fill in is big enough
				//	// coz we need terminating zero. 

				//	// CreateProcessW is asci code to build issue you can also use CreateProcess() also. 
				//	auto res = CreateProcess(
				//		NULL,//application name is null since its provided by next param(be carefull doesnt use path, so needs full freaking path whole path)
				//		commandline,  // could expand
				//		NULL,  // same as parent
				//		NULL,
				//		false,  // so if one closes the other could open, one security breaches the other one could shut it down 
				//		0,   // normal
				//		NULL,  // if null takes what ever it has in enviourment variables// you can also fake env with better or less options
				//		NULL,  // working directory
				//		&sinfo,
				//		&pi
				//	);

				//	if (res == 0) {
				//		cerr << "Error : " << GetLastError() << endl;
				//	}

				//	delete[] commandline;
				//}
				//catch (std::bad_alloc) {
				//	wcerr << L"Insufficient memroy to launch application \n";
				//	return;
				//}

				// after creating the process delay it for the time
		/*	}
		}*/


	
		// exit createprocess here
	}

	// once loop finish end again



	// 2  Read input
	// 2  comma sepreateed
	// 2  Read parameter
	// 2  once '\n' end of line.




	for (int i = 1; i <= tmpArgs.size(); ++i) {

	}
	
	



	//
	

	
	return 0;
}
