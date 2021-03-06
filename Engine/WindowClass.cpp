#include "WindowClass.h"
#include "ProgramRootClass.h"
#include "GenericException.h"

using namespace std;

WindowClass* WindowClass::m_pInstance = nullptr;
mutex WindowClass::instanceMutex;

WindowClass:: WindowClass()
{
    m_hWnd = nullptr;
    m_hInstance = nullptr;
}

WindowClass::WindowClass(const WindowClass& other)
{
}

WindowClass::~WindowClass()
{
}

WindowClass* WindowClass::getInstance()
{
    instanceMutex.lock();
    if (!m_pInstance)
        m_pInstance = new WindowClass();
    instanceMutex.unlock();

    return m_pInstance;
}

void WindowClass::Initialize(HINSTANCE hInstance, int nCmdShow)
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
    wcex.hIcon = LoadIcon( nullptr, IDI_WINLOGO );
    wcex.hIconSm = wcex.hIcon;
    wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"NewGame";
    
    if (!RegisterClassEx(&wcex))
    {
        throw GenericException("Failed to register the window class.");
    }

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
                            posX, posY, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, m_hInstance, nullptr);

    if (m_hWnd == nullptr)
    {
        throw GenericException("Failed to create the window.");
    }

    ShowWindow( m_hWnd, nCmdShow );
    SetForegroundWindow(m_hWnd);
    SetFocus(m_hWnd);
}

void WindowClass::Shutdown()
{
    return;
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