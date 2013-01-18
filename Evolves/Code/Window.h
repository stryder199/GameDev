//=================================
// include guard
#ifndef Window_H_
#define Window_H_

//=================================
// forward declared dependencies


//=================================
// included dependencies
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

class Window
{

private:
	HINSTANCE g_hInstance;
	HWND      g_hWnd;

public:
	Window();
	HRESULT init(HINSTANCE hInstance, int nCmdShow);
	HWND gethWnd();
};

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

#endif /* Window_H_ */