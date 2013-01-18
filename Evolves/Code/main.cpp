#include "ProgramRoot.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

	ProgramRoot *root = new ProgramRoot();

	int exitCode = root->go(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	return exitCode;
}