#pragma once

//////////////
// INCLUDES //
#include <Windows.h>
#include <future>

#define Async

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class SoundClass;
class GraphicsClass;
class EventClass;

class ProgramRootClass{

public:
    ProgramRootClass();
    ~ProgramRootClass();
    
    void Initialize( HINSTANCE hInstance, int iCmdshow );
    void Shutdown();
    void Render();
    void Go();

private:
    SoundClass* m_Sounds;
    GraphicsClass* m_Graphics;
    EventClass* m_Events;
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
