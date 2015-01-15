#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

static class WindowsHelpers
{
 public:
    static string DesktopDir();
    static string SkyDriveDir();
    static LPCWSTR ToLWideStr(string s);
    static LPCTSTR ToLTStr(string s);
    static vector<string> ListDir(string dirPath);
    static vector<string> ListFile(string dirPath, string extension);
    static TCHAR* ToTStr(string s);
    static bool FileExists(const string fileName);
    static void CopyFilesFromDir(string sourceDir, string destDir, bool overrideFiles, bool recursive, bool skipNewerFiles);
    static bool CheckForNewerVersion(string sourceFilePath, string destFilePath);
};
