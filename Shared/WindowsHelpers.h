#include <vector>
#include <Windows.h>

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
    static std::string DesktopDir();
    static std::string SkyDriveDir();
    static LPCWSTR ToLWideStr(std::string s);
    static LPCTSTR ToLTStr(std::string s);
    static std::vector<std::string> ListDir(std::string dirPath);
    static std::vector<std::string> ListFile(std::string dirPath, std::string extension);
    static TCHAR* ToTStr(std::string s);
    static bool FileExists(const std::string fileName);
    static void CopyFilesFromDir(std::string sourceDir, std::string destDir, bool overrideFiles, bool recursive, bool skipNewerFiles);
    static NewerFile CheckForNewerVersion(std::string sourceFilePath, std::string destFilePath);
    static void BuildFolderPaths(std::string path);
    static float RandomFloat(float a, float b);
    static bool FolderExists(const std::string folderName);
    static std::string CurrentExePath();
};