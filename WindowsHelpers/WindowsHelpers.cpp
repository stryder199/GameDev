#include "WindowsHelpers.h"
#include <Windows.h>
#include <shlobj.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

string WindowsHelpers::DesktopDir()
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

string WindowsHelpers::SkyDriveDir()
{
    static char path[MAX_PATH + 1];
    if (SHGetSpecialFolderPathA(HWND_DESKTOP, path, CSIDL_PROFILE, FALSE))
    {
        string s = path;
        s = s + "\\SkyDrive";
        return s;
    }
    else
        return "ERROR";
}

LPCWSTR WindowsHelpers::ToLWideStr(string s)
{
    wstring* stemp = new wstring(s.begin(), s.end());
    LPCWSTR lpathWildcard = stemp->c_str();
    return lpathWildcard;
}

LPCTSTR WindowsHelpers::ToLTStr(string s)
{
    return (LPCTSTR)s.c_str();
}

vector<string> WindowsHelpers::ListDir(string dirPath)
{
    vector<string> dirList = vector<string>();
    HANDLE dir;
    WIN32_FIND_DATA file_data;

    string pathWildcard = dirPath + "\\*";
    wstring stemp = wstring(pathWildcard.begin(), pathWildcard.end());
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
        string filename(ch);

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

vector<string> WindowsHelpers::ListFile(string dirPath, string extension)
{
    HANDLE hFind;
    WIN32_FIND_DATA data;
    vector<string> fileList;

    string pathWildcard = dirPath + "\\*." + extension;
    wstring stemp = wstring(pathWildcard.begin(), pathWildcard.end());
    LPCWSTR lpathWildcard = stemp.c_str();

    hFind = FindFirstFile(lpathWildcard, &data);
    if (hFind != INVALID_HANDLE_VALUE) 
    {
        do 
        {
            WCHAR* wc = data.cFileName;

            //convert from wide char to narrow char array
            char ch[260];
            char DefChar = ' ';
            WideCharToMultiByte(CP_ACP, 0, wc, -1, ch, 260, &DefChar, NULL);

            //A std:string  using the char* constructor.
            std::string ss(ch);
            if (ss != "." || ss != "..")
            {
                fileList.push_back(ss);
            }
        } while (FindNextFile(hFind, &data));

        FindClose(hFind);
    }

    return fileList;
}

TCHAR* WindowsHelpers::ToTStr(string s)
{
    TCHAR *param = new TCHAR[s.size() + 1];
    param[s.size()] = 0;
    copy(s.begin(), s.end(), param);
    return param;
}

bool WindowsHelpers::FileExists(const string fileName)
{
    DWORD fileAttr;

    fileAttr = GetFileAttributes(ToTStr(fileName));
    if (0xFFFFFFFF == fileAttr)
        return false;
    return true;
}

void WindowsHelpers::CopyFilesFromDir(string sourceDir, string destDir, bool overrideFiles, bool recursive, bool skipNewerFiles)
{
    vector<string> listFile = ListFile(sourceDir, "*");
    for (string file : listFile)
    {
        string sourceFile = sourceDir + "\\" + file;
        string destFile = destDir + "\\" + file;
        LPCWSTR wSourceFile = ToLWideStr(sourceFile);
        LPCWSTR wDestFile = ToLWideStr(destFile);

        if (!FileExists(destFile))
        {
            cout << "Copying " << sourceFile << " to " << destFile << endl;
            CopyFile(wSourceFile, wDestFile, overrideFiles);

        }
        else
        {
            if (skipNewerFiles)
            {
                if (CheckForNewerVersion(sourceFile, destFile))
                {
                    cout << "Copying " << sourceFile << " to " << destFile << endl;
                    CopyFile(wSourceFile, wDestFile, overrideFiles);
                }
            }
            else
            {
                cout << "Copying " << sourceFile << " to " << destFile << endl;
                CopyFile(wSourceFile, wDestFile, overrideFiles);
            }
            
        }
    }

    vector<string> folderList = ListDir(sourceDir);
    for (string dir : folderList)
    {
        string sourceFolder = sourceDir + "\\" + dir;
        string destFolder = destDir + "\\" + dir;
        CopyFilesFromDir(sourceFolder, destFolder, overrideFiles, recursive, skipNewerFiles);
    }
}

// Returns true if file one is newer than file two
bool WindowsHelpers::CheckForNewerVersion(string firstFilePath, string secondFilePath)
{
    bool result = false;
    if (WindowsHelpers::FileExists(secondFilePath))
    {
        FILETIME ftCreateOut, ftAccessOut, ftWriteOut, ftCreateIn, ftAccessIn, ftWriteIn;

        HANDLE inFile = CreateFile(ToLWideStr(firstFilePath), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        HANDLE outFile = CreateFile(ToLWideStr(secondFilePath), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

        if (!GetFileTime(outFile, &ftCreateOut, &ftAccessOut, &ftWriteOut))
            return false;

        if (!GetFileTime(inFile, &ftCreateIn, &ftAccessIn, &ftWriteIn))
            return false;

        if (CompareFileTime(&ftWriteIn, &ftWriteOut) > 0)
        {
            // File one is newer
            result = true;
        }
        CloseHandle(outFile);
        CloseHandle(inFile);
    }

    return result;
}