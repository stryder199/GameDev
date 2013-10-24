#include "WindowClass.h"
#include "ProgramRootClass.h"

WindowClass* WindowClass::m_pInstance = NULL;

WindowClass:: WindowClass()
{
	m_hWnd = 0;
	m_hInstance = 0;
}

WindowClass::WindowClass(const WindowClass& other)
{
}

WindowClass::~WindowClass()
{
}

WindowClass* WindowClass::getInstance()
{
	if (!m_pInstance)
		m_pInstance = new WindowClass();

	return m_pInstance;
}

bool WindowClass::Initialize(HINSTANCE hInstance, int nCmdShow)
{
	DEVMODE dmScreenSettings;
	int posX, posY;

	m_hInstance = hInstance;
	FULL_SCREEN = false;
	VSYNC_ENABLED = true;
	SCREEN_DEPTH = 1000.0f;
	SCREEN_NEAR = 0.1f;
	SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
	SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);

	// Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = (WNDPROC)WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon( NULL, IDI_WINLOGO );
	wcex.hIconSm = wcex.hIcon;
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"NewGame";
    
    if( !RegisterClassEx( &wcex ) )
        return false;

	WINDOW_WIDTH  = SCREEN_WIDTH;
	WINDOW_HEIGHT = SCREEN_HEIGHT;

    // Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)WINDOW_WIDTH;
		dmScreenSettings.dmPelsHeight = (unsigned long)WINDOW_HEIGHT;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		WINDOW_WIDTH  = 1280;
		WINDOW_HEIGHT = 720;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - WINDOW_WIDTH)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - WINDOW_HEIGHT) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, L"NewGame", L"NewGame", 
						    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							posX, posY, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, m_hInstance, NULL);
    if( !m_hWnd )
        return false;

    ShowWindow( m_hWnd, nCmdShow );
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

    return true;
}

HWND WindowClass::gethWnd()
{
	return m_hWnd;
}

int WindowClass::getScreenWidth()
{
	return SCREEN_WIDTH;
}

int WindowClass::getScreenHeight()
{
	return SCREEN_HEIGHT;
}

int WindowClass::getWindowWidth()
{
	return WINDOW_WIDTH;
}

int WindowClass::getWindowHeight()
{
	return WINDOW_HEIGHT;
}