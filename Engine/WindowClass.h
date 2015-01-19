#pragma once

//////////////
// INCLUDES //
#include <Windows.h>
#include <mutex>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //

using namespace std;

class WindowClass{

public:
    ~WindowClass();
    static WindowClass* getInstance();

    void Initialize(HINSTANCE hInstance, int nCmdShow);
    void Shutdown();

    HWND gethWnd();
    int getScreenHeight();
    int getScreenWidth();
    int getWindowHeight();
    int getWindowWidth();

private:
    WindowClass();
    WindowClass(const WindowClass&);
    WindowClass& operator=(const WindowClass&) {};

    static WindowClass* m_pInstance;
    static mutex instanceMutex;

    HINSTANCE m_hInstance;
    HWND m_hWnd;
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;

public:
    bool FULL_SCREEN;
    bool VSYNC_ENABLED;
    float SCREEN_DEPTH;
    float SCREEN_NEAR;

};