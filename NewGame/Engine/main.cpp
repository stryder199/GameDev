////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ProgramRootClass.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{	
	// Create the system object.
    ProgramRootClass* programRoot = new ProgramRootClass();

	// Initialize and run the system object.
	programRoot->Initialize(hInstance, iCmdshow);

	programRoot->Go();

	// Shutdown and release the system object.
	programRoot->Shutdown();
	delete programRoot;
	programRoot = 0;

	return 0;
}