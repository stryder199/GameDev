#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

class WindowsHelpers
{
public:
    enum NewerFile
    {
        FileOneIsNewer,
        FileTwoIsNewer,
        FileTwoDoesntExist
    };

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
    static NewerFile CheckForNewerVersion(string sourceFilePath, string destFilePath);
    static void BuildFolderPaths(std::string path);
    static float RandomFloat(float a, float b);
    static bool FolderExists(const string folderName);
    static string CurrentExePath();
};