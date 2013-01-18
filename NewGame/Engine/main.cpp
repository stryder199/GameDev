////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ProgramRootClass.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	ProgramRootClass* programRoot;
	bool result;
	
	
	// Create the system object.
	programRoot = new ProgramRootClass();
	if(!programRoot)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = programRoot->Initialize(hInstance, iCmdshow);
	if(result)
	{
		programRoot->Go();
	}

	// Shutdown and release the system object.
	programRoot->Shutdown();
	delete programRoot;
	programRoot = 0;

	return 0;
}