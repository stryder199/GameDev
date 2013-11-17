#include "ProgramRootClass.h"
#include "EventClass.h"
#include "GraphicsClass.h"
#include "SoundClass.h"
#include "WindowClass.h"
#include "CameraClass.h"
#include "PlayerClass.h"

ProgramRootClass::ProgramRootClass()
{
	m_Events = 0;
	m_Graphics = 0;
	m_Sounds = 0;
}

ProgramRootClass::~ProgramRootClass()
{
}

bool ProgramRootClass::Go()
{
	MSG msg;
	bool done, result;
	bool gameLoaderWaiterDone = false;

	auto gameLoader = async(launch::async, &GraphicsClass::LoadGameData, m_Graphics);

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Get the result from the game loader
		if (!gameLoaderWaiterDone && gameLoader.wait_for(chrono::seconds(0)) == future_status::ready)
		{
			gameLoaderWaiterDone = true;
			result = gameLoader.get();
			if (!result)
				return false;
		}

		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.  If frame processing fails then exit.
			result = Render();
			if (!result)
			{
				MessageBox(WindowClass::getInstance()->gethWnd(), L"Frame Processing Failed", L"Error", MB_OK);
				done = true;
			}
		}

		// Check if the user pressed escape and wants to quit.
		if (m_Events->IsEscapePressed() == true)
		{
			done = true;
		}

		if (m_Events->IsWPressed() == true)
		{
			PlayerClass::getInstance()->SetEnginePower(1.0f);
		}
		else if (m_Events->IsSPressed() == true)
		{
			PlayerClass::getInstance()->SetEnginePower(-1.0f);
		}
		else{
			PlayerClass::getInstance()->SetEnginePower(0.0f);
		}

		if (m_Events->IsDPressed() == true)
		{
			PlayerClass::getInstance()->SetRotVelY(XM_PI / 100);
		}
		else if (m_Events->IsAPressed() == true)
		{
			PlayerClass::getInstance()->SetRotVelY(-1.0f * XM_PI / 100);
		}
		else{
			PlayerClass::getInstance()->SetRotVelY(0.0f);
		}

		if (m_Events->IsSpacePressed())
		{
			PlayerClass::getInstance()->StartWeaponFiring();
		}
	}

	return true;
}

bool ProgramRootClass::Initialize( HINSTANCE hInstance, int iCmdshow )
{
	bool result;

	result = WindowClass::getInstance()->Initialize(hInstance, iCmdshow);
	if (!result)
		return false;

	m_Sounds = new SoundClass();
	if(!m_Sounds)
		return false;

	result = m_Sounds->Initialize();
	if(!result)
		return false;
	
	m_Events = new EventClass();
	if(!m_Events)
		return false;

	result = m_Events->Initialize( hInstance );
	if(!result)
		return false;

	m_Graphics = new GraphicsClass();
	if (!m_Graphics)
		return false;

	result = m_Graphics->Initialize(hInstance, iCmdshow);
	if (!result)
		return false;

	return true;
}

bool ProgramRootClass::Shutdown()
{
	return true;
}

bool ProgramRootClass::Render()
{
	bool result;

	result = m_Events->Render();
	if(!result)
		return false;

	result = m_Graphics->Render();
	if(!result)
		return false;
	return true;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}
