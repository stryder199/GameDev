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

bool CheckForNewerVersion(string filename, string inputFolder, string outputFolder)
{
	string inputFullPath = inputFolder + "\\" + filename;
	string outputFullPath = outputFolder + "\\" + filename;
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

		if (CheckForNewerVersion((*it), INPUT_FOLDER, OUTPUT_FOLDER))
		{
			// Skip this file
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
	vector<string> dirlist = listDir(INPUT_FOLDER);
	vector<string>::iterator dir;
	for (dir = dirlist.begin(); dir != dirlist.end(); ++dir)
	{
		string command1 = "xcopy /E /Q /Y /I " + INPUT_FOLDER + "\\" + (*dir) + " " + OUTPUT_FOLDER + "\\" + (*dir);
		system(command1.c_str());
	}

	
	string command2 = "xcopy /E /Q /Y /I " + OUTPUT_FOLDER + "\\* " + GAME_DATA_FOLDER + "\\";

	system(command2.c_str());

	// Notify the user the model has been converted.
	cout << "\nDo you wish to exit (y/n)? ";
	cin >> garbage;

	return 0;
}