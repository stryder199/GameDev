//=================================
// include guard
#pragma once

//=================================
// forward declared dependencies
class SoundClass;
class GraphicsClass;
class EventClass;
class CameraClass;

//=================================
// included dependencies
#include <Windows.h>

class ProgramRootClass{

public:
	ProgramRootClass();
	ProgramRootClass(const ProgramRootClass&);
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
