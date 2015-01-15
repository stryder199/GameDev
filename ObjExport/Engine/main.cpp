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



//////////////////
// MAIN PROGRAM //
//////////////////
int main()
{
	bool result;
	char garbage;

	vector<string> filenameList = WindowsHelpers::ListFile(INPUT_FOLDER, "obj");

	vector<string>::iterator it;

	for (it = filenameList.begin(); it != filenameList.end(); ++it)
	{
		time_t start, end;
		double seconds;

        string modelFileName = (*it);
        string inputFullPath = INPUT_FOLDER + "\\" + (*it);
        string outputFullPath = OUTPUT_FOLDER + "\\" + (*it);
        outputFullPath.erase(outputFullPath.end() - 4, outputFullPath.end());
        outputFullPath += ".3dmodel";

        if (!WindowsHelpers::CheckForNewerVersion(inputFullPath, outputFullPath))
		{
            cout << "Skipping " << inputFullPath << ". A newer version exists!" << endl;
			// Skip this file
			continue;
		}

		Exporter* ex = new Exporter();
		cout << "Starting Export of " << (*it) << endl;
		time(&start);
		// Now read the data from the file into the data structures and then output it in our model format.
		ex->LoadDataStructures((*it));
		time(&end);
		seconds = difftime(end, start);
		cout << "Finished in: " << (int) seconds << " seconds." << endl << endl;
		if (!result)
		{
			return -1;
		}

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