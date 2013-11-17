#pragma once

//////////////
// INCLUDES //
#include <Windows.h>
#include <future>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class SoundClass;
class GraphicsClass;
class EventClass;

using namespace std;

class ProgramRootClass{

public:
	ProgramRootClass();
	~ProgramRootClass();


	bool Go();
	bool Shutdown();
	bool Initialize( HINSTANCE hInstance, int iCmdshow );
	bool Render();

private:
	SoundClass* m_Sounds;
	GraphicsClass* m_Graphics;
	EventClass* m_Events;
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
