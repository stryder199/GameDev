#include "WindowsHelpers.h"
#include <Windows.h>
#include <shlobj.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "GenericException.h"
#include "boost\algorithm\string.hpp"
#include "boost\filesystem\operations.hpp"
#include "boost\filesystem\path.hpp"
#include <iostream>

using namespace boost;
using namespace boost::filesystem;
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
        WideCharToMultiByte(CP_ACP, 0, wc, -1, ch, 260, &DefChar, nullptr);

        //A std:string  using the char* constructor.
        string filename(ch);

        bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

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
            WideCharToMultiByte(CP_ACP, 0, wc, -1, ch, 260, &DefChar, nullptr);

            bool is_directory = (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

            if (!is_directory)
            {
                //A std:string  using the char* constructor.
                string ss(ch);
                if (ss != "." && ss != "..")
                {
                    fileList.push_back(ss);
                }
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
    DWORD fileAttr = GetFileAttributes(ToTStr(fileName));
    if (0xFFFFFFFF == fileAttr)
        return false;
    return true;
}

bool WindowsHelpers::FolderExists(const string folderName)
{
    DWORD dwAttrib = GetFileAttributes(ToTStr(folderName));

    return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void WindowsHelpers::CopyFilesFromDir(string sourceDir, string destDir, bool overrideFiles, bool recursive, bool skipNewerFiles)
{
    vector<string> listFile = ListFile(sourceDir, "*");
    if (listFile.size() > 0)
    {
        BuildFolderPaths(destDir);
        for (string &file : listFile)
        {
            string sourceFile = sourceDir + "\\" + file;
            string destFile = destDir + "\\" + file;

            if (skipNewerFiles)
            {
                if (CheckForNewerVersion(sourceFile, destFile) == FileTwoIsNewer)
                {
                    cout << "Skipping " << sourceFile << ". A newer version of 3dmodel exists!" << endl;
                    // Skip this file
                    continue;
                }
                cout << "Copying " << sourceFile << " to " << destFile << endl;
                BOOL result = CopyFile(ToLWideStr(sourceFile), ToLWideStr(destFile), overrideFiles);
                if (!result)
                {
                    DWORD error = GetLastError();
                    throw GenericException("Didn't copy files " + error);
                }
            }
            else
            {
                cout << "Copying " << sourceFile << " to " << destFile << endl;
                BOOL result = CopyFile(ToLWideStr(sourceFile), ToLWideStr(destFile), overrideFiles);
                if (!result)
                {
                    DWORD error = GetLastError();
                    throw GenericException("Didn't copy files " + error);
                }
            }

        }
    }    

    vector<string> folderList = ListDir(sourceDir);
    for (string &dir : folderList)
    {
        string sourceFolder = sourceDir + "\\" + dir;
        string destFolder = destDir + "\\" + dir;
        CopyFilesFromDir(sourceFolder, destFolder, overrideFiles, recursive, skipNewerFiles);
    }
}

void WindowsHelpers::BuildFolderPaths(string path)
{
    vector<string> SplitVec;
    split(SplitVec, path, is_any_of("\\"), token_compress_on);
    string segmentPath = "";
    for (string &dir : SplitVec)
    {
        segmentPath = segmentPath + dir + "\\";
        if (!FolderExists(segmentPath))
        {
            int result = CreateDirectory(ToLWideStr(segmentPath), nullptr);
            if (result == 0)
            {
                throw GenericException("Failed to create dir");
            }
        }
    }
}

/// Return 1 if file 1 is newer, return 2 if file 2 is newer, return -1 on error
WindowsHelpers::NewerFile WindowsHelpers::CheckForNewerVersion(string firstFilePath, string secondFilePath)
{
    NewerFile result = NewerFile::FileTwoDoesntExist;
    if (!WindowsHelpers::FileExists(firstFilePath))
    {
        throw GenericException("First file not found");
    }

    if (WindowsHelpers::FileExists(secondFilePath))
    {
        FILETIME ftCreateOut, ftAccessOut, ftWriteOut, ftCreateIn, ftAccessIn, ftWriteIn;

        HANDLE inFile = CreateFile(ToLWideStr(firstFilePath), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
        HANDLE outFile = CreateFile(ToLWideStr(secondFilePath), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);

        if (!GetFileTime(outFile, &ftCreateOut, &ftAccessOut, &ftWriteOut))
            return NewerFile::FileTwoDoesntExist;

        if (!GetFileTime(inFile, &ftCreateIn, &ftAccessIn, &ftWriteIn))
            return NewerFile::FileTwoDoesntExist;

        if (CompareFileTime(&ftWriteIn, &ftWriteOut) > 0)
        {
            // File one is newer
            result = NewerFile::FileOneIsNewer;
        }
        else
        {
            result = NewerFile::FileTwoIsNewer;
        }
        CloseHandle(outFile);
        CloseHandle(inFile);
    }

    return result;
}

float WindowsHelpers::RandomFloat(float a, float b)
{
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}


string WindowsHelpers::CurrentExePath()
{
    return current_path().string();
}