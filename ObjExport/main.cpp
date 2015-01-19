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
#include "..\Shared\WindowsHelpers.h"
#include <time.h>

using namespace std;


//////////////////
// MAIN PROGRAM //
//////////////////
int main()
{
    char garbage;

    vector<string> filenameList = WindowsHelpers::ListFile(INPUT_FOLDER, "obj");

    for (string &filename : filenameList)
    {
        time_t start, end;
        double seconds;

        string modelFileName = filename;
        string inputFullPath = INPUT_FOLDER + "\\" + filename;
        string outputFullPath = OUTPUT_FOLDER + "\\" + filename;
        outputFullPath.erase(outputFullPath.end() - 4, outputFullPath.end());
        outputFullPath += ".3dmodel";

        if (WindowsHelpers::CheckForNewerVersion(inputFullPath, outputFullPath) == WindowsHelpers::FileTwoIsNewer)
        {
            cout << "Skipping " << inputFullPath << ". A newer version of 3dmodel exists!" << endl;
            // Skip this file
            continue;
        }

        Exporter* ex = new Exporter();
        cout << "Starting Export of " << filename << endl;
        time(&start);
        // Now read the data from the file into the data structures and then output it in our model format.
        ex->LoadDataStructures(filename);
        time(&end);
        seconds = difftime(end, start);
        cout << "Finished in: " << (int) seconds << " seconds." << endl << endl;
    }
    vector<string> dirlist = WindowsHelpers::ListDir(INPUT_FOLDER);
    for (vector<string>::iterator dir = dirlist.begin(); dir != dirlist.end(); ++dir)
    {
        string sourceDir = INPUT_FOLDER + "\\" + (*dir);
        string destDir = OUTPUT_FOLDER + "\\" + (*dir);
        
        WindowsHelpers::CopyFilesFromDir(sourceDir, destDir, true, true, true);
    }

    WindowsHelpers::CopyFilesFromDir(OUTPUT_FOLDER, GAME_DATA_FOLDER, true, true, true);

    // Notify the user the model has been converted.
    cout << "\nDo you wish to exit (y/n)? ";
    cin >> garbage;

    return 0;
}