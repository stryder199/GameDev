//=================================
// include guard
#pragma once

//=================================
// forward declared dependencies


//=================================
// included dependencies
#include <Windows.h>

class WindowClass{

public:
	~WindowClass();
	static WindowClass* getInstance();

	bool Initialize(HINSTANCE hInstance, int nCmdShow);
	HWND gethWnd();
	int getScreenHeight();
	int getScreenWidth();
	int getWindowHeight();
	int getWindowWidth();

private:
	WindowClass();
	WindowClass(const WindowClass&);
	WindowClass& operator=(WindowClass& const) {};

private:
	static WindowClass* m_pInstance;
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