////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////


//////////////
// INCLUDES //
//////////////
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "Exporter.h"
#include <time.h>

using namespace std;

vector<string> listFile(string dirPath, string extension);
LPCWSTR toLWideStr(string s);

bool CheckForNewerVersion(string filename)
{
	string inputFullPath = INPUT_FOLDER + "\\" + filename;
	string outputFullPath = OUTPUT_FOLDER + "\\" + filename;
	outputFullPath.erase(outputFullPath.end() - 4, outputFullPath.end());
	outputFullPath += ".3dmodel";

	if (FileExists(outputFullPath))
	{
		FILETIME ftCreateOut, ftAccessOut, ftWriteOut, ftCreateIn, ftAccessIn, ftWriteIn;

		wstring stemp = std::wstring(outputFullPath.begin(), outputFullPath.end());
		LPCWSTR out = stemp.c_str();

		wstring stemp2 = std::wstring(inputFullPath.begin(), inputFullPath.end());
		LPCWSTR in = stemp2.c_str();

		HANDLE outFile = CreateFile(out, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		HANDLE inFile = CreateFile(in, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

		if (!GetFileTime(outFile, &ftCreateOut, &ftAccessOut, &ftWriteOut))
			return false;

		if (!GetFileTime(inFile, &ftCreateIn, &ftAccessIn, &ftWriteIn))
			return false;

		LONG longresult = CompareFileTime(&ftWriteIn, &ftWriteOut);

		if (longresult < 0)
		{
			cout << "Skipping " << filename << ". A newer version exists!" << endl;
			CloseHandle(outFile);
			CloseHandle(inFile);
			return true;
		}
		CloseHandle(outFile);
		CloseHandle(inFile);
	}

	return false;
}

vector<string> listFile(string dirPath, string extension)
{
	HANDLE hFind;
	WIN32_FIND_DATA data;
	vector<string> fileList;

	string pathWildcard = dirPath + "\\*." + extension;
	std::wstring stemp = std::wstring(pathWildcard.begin(), pathWildcard.end());
	LPCWSTR lpathWildcard = stemp.c_str();

	hFind = FindFirstFile(lpathWildcard, &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			WCHAR* wc = data.cFileName;

			//convert from wide char to narrow char array
			char ch[260];
			char DefChar = ' ';
			WideCharToMultiByte(CP_ACP, 0, wc, -1, ch, 260, &DefChar, NULL);

			//A std:string  using the char* constructor.
			std::string ss(ch);

			fileList.push_back(ss);
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}

	return fileList;
}

LPCWSTR toLWideStr(string s)
{
	wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR lpathWildcard = stemp.c_str();
	return lpathWildcard;
}

LPCTSTR toLTStr(string s)
{
	return (LPCTSTR) s.c_str();
}

//////////////////
// MAIN PROGRAM //
//////////////////
int main()
{
	bool result;
	char garbage;

	vector<string> filenameList = listFile(INPUT_FOLDER, "obj");

	vector<string>::iterator it;

	for (it = filenameList.begin(); it != filenameList.end(); ++it)
	{
		time_t start, end;
		double seconds;

		if (CheckForNewerVersion((*it)))
		{
			continue;
		}

		Exporter* ex = new Exporter();
		cout << "Starting Export of " << (*it) << endl;
		time(&start);
		// Now read the data from the file into the data structures and then output it in our model format.
		result = ex->LoadDataStructures((*it));
		time(&end);
		seconds = difftime(end, start);
		cout << "Finished in: " << (int) seconds << " seconds." << endl << endl;
		if (!result)
		{
			return -1;
		}

	}

	// Notify the user the model has been converted.
	cout << "\nDo you wish to exit (y/n)? ";
	cin >> garbage;

	return 0;
}