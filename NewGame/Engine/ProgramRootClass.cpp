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

void ProgramRootClass::Go()
{
	MSG msg;
	bool done;
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
			gameLoader.get();
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
			Render();
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
		else
        {
			PlayerClass::getInstance()->SetRotVelY(0.0f);
		}

		if (m_Events->IsSpacePressed())
		{
			PlayerClass::getInstance()->StartWeaponFiring();
		}
	}
}

void ProgramRootClass::Initialize( HINSTANCE hInstance, int iCmdshow )
{
	WindowClass::getInstance()->Initialize(hInstance, iCmdshow);

	m_Sounds = new SoundClass();
	m_Sounds->Initialize();
	
	m_Events = new EventClass();
	m_Events->Initialize( hInstance );

	m_Graphics = new GraphicsClass();
	m_Graphics->Initialize(hInstance, iCmdshow);
}

void ProgramRootClass::Shutdown()
{
	if (m_Sounds)
	{
		m_Sounds->Shutdown();
		delete m_Sounds;
		m_Sounds = 0;
	}
	
	if (m_Events)
	{
		m_Events->Shutdown();
		delete m_Events;
		m_Events = 0;
	}

	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	WindowClass::getInstance()->Shutdown();
	delete WindowClass::getInstance();
}

void ProgramRootClass::Render()
{
	m_Events->Render();

	m_Graphics->Render();
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
