#include "ProgramRootClass.h"
#include "EventClass.h"
#include "GarphicsClass.h"
#include "SoundClass.h"
#include "WindowClass.h"
#include "CameraClass.h"

ProgramRootClass::ProgramRootClass()
{
	m_Events = 0;
	m_Graphics = 0;
	m_Sounds = 0;
}

ProgramRootClass::ProgramRootClass(const ProgramRootClass& other)
{
}

ProgramRootClass::~ProgramRootClass()
{
}

bool ProgramRootClass::Go()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	// Loop until there is a quit message from the window or the user.
	done = false;
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.  If frame processing fails then exit.
			result = Render();
			if(!result)
			{
				//MessageBox(m_hwnd, L"Frame Processing Failed", L"Error", MB_OK);
				done = true;
			}
		}

		// Check if the user pressed escape and wants to quit.
		if(m_Events->IsEscapePressed() == true)
		{
			done = true;
		}

		if(m_Events->IsUpPressed() == true)
		{
			m_Graphics->getCamera()->setVelY(1.0f);
		}
		else if(m_Events->IsDownPressed() == true)
		{
			m_Graphics->getCamera()->setVelY(-1.0f);
		}
		else{
			m_Graphics->getCamera()->setVelY(0.0f);
		}

		if(m_Events->IsRightPressed() == true)
		{
			m_Graphics->getCamera()->setVelX(1.0f);
		}
		else if(m_Events->IsLeftPressed() == true)
		{
			m_Graphics->getCamera()->setVelX(-1.0f);
		}
		else{
			m_Graphics->getCamera()->setVelX(0.0f);
		}

		if(m_Events->IsRightClickPressed() == true)
		{
			m_Graphics->getCamera()->setVelZ(1.0f);
		}
		else if(m_Events->IsLeftClickPressed() == true)
		{
			m_Graphics->getCamera()->setVelZ(-1.0f);
		}
		else{
			m_Graphics->getCamera()->setVelZ(0.0f);
		}

		if(m_Events->IsDPressed() == true)
		{
			m_Graphics->getCamera()->setVelRotX(1.0f);
		}
		else if(m_Events->IsAPressed() == true)
		{
			m_Graphics->getCamera()->setVelRotX(-1.0f);
		}
		else{
			m_Graphics->getCamera()->setVelRotX(0.0f);
		}

		if(m_Events->IsWPressed() == true)
		{
			m_Graphics->getCamera()->setVelRotY(1.0f);
		}
		else if(m_Events->IsSPressed() == true)
		{
			m_Graphics->getCamera()->setVelRotY(-1.0f);
		}
		else{
			m_Graphics->getCamera()->setVelRotY(0.0f);
		}

		if(m_Events->IsEPressed() == true)
		{
			m_Graphics->getCamera()->setVelRotZ(1.0f);
		}
		else if(m_Events->IsQPressed() == true)
		{
			m_Graphics->getCamera()->setVelRotZ(-1.0f);
		}
		else{
			m_Graphics->getCamera()->setVelRotZ(0.0f);
		}
	}

	return true;
}

bool ProgramRootClass::Initialize( HINSTANCE hInstance, int iCmdshow )
{
	bool result;	

	m_Sounds = new SoundClass();
	if(!m_Sounds)
		return false;

	result = m_Sounds->Initialize();
	if(!result)
		return false;

	m_Graphics = new GraphicsClass();
	if(!m_Graphics)
		return false;

	result = m_Graphics->Initialize(hInstance, iCmdshow);
	if(!result)
		return false;
	
	m_Events = new EventClass();
	if(!m_Events)
		return false;

	result = m_Events->Initialize( hInstance, m_Graphics->getWindowObj()->gethWnd(),  m_Graphics->getWindowObj()->getScreenWidth(), m_Graphics->getWindowObj()->getScreenWidth() );
	if(!result)
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
