#include <Windows.h>
#include <shlobj.h>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;


static string desktopDir()
{
	static char path[MAX_PATH + 1];
	if (SHGetSpecialFolderPathA(HWND_DESKTOP, path, CSIDL_DESKTOPDIRECTORY, FALSE))
	{
		string s = path;
		return s;
	}
	else
		return "ERROR";
}

static LPCWSTR toLWideStr(string s)
{
	wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR lpathWildcard = stemp.c_str();
	return lpathWildcard;
}

static LPCTSTR toLTStr(string s)
{
	return (LPCTSTR) s.c_str();
}

static vector<string> listDir(string dirPath)
{
	vector<string> dirList = vector<string>();
	HANDLE dir;
	WIN32_FIND_DATA file_data;

	string pathWildcard = dirPath + "\\*";
	std::wstring stemp = std::wstring(pathWildcard.begin(), pathWildcard.end());
	LPCWSTR lpathWildcard = stemp.c_str();

	if ((dir = FindFirstFile(lpathWildcard, &file_data)) == INVALID_HANDLE_VALUE)
	return dirList; /* No files found */

	do {
		WCHAR* wc = file_data.cFileName;

		//convert from wide char to narrow char array
		char ch[260];
		char DefChar = ' ';
		WideCharToMultiByte(CP_ACP, 0, wc, -1, ch, 260, &DefChar, NULL);

		//A std:string  using the char* constructor.
		std::string filename(ch);

		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (filename[0] == '.')
			continue;

		if (!is_directory)
			continue;

		dirList.push_back(filename);
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);

	return dirList;
}

static vector<string> listFile(string dirPath, string extension)
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

static TCHAR* toTStr(string s)
{
	TCHAR *param = new TCHAR[s.size() + 1];
	param[s.size()] = 0;
	copy(s.begin(), s.end(), param);
	return param;
}

static bool FileExists(const string fileName)
{
	DWORD       fileAttr;

	fileAttr = GetFileAttributes(toTStr(fileName));
	if (0xFFFFFFFF == fileAttr)
		return false;
	return true;
}