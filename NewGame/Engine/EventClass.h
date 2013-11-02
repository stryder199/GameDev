//=================================
// include guard
#pragma once

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800

//constants for mouse buttons (NEW)
#define DIMOUSE_LEFTBUTTON   0
#define DIMOUSE_RIGHTBUTTON  1
#define DIMOUSE_MIDDLEBUTTON 2
#define DIMOUSE_4BUTTON      3
#define DIMOUSE_5BUTTON      4
#define DIMOUSE_6BUTTON      5
#define DIMOUSE_7BUTTON      6
#define DIMOUSE_8BUTTON      7

//=================================
// forward declared dependencies


//=================================
// included dependencies
#include <dinput.h>

class EventClass{

public:
	EventClass();
	EventClass(const EventClass&);
	~EventClass();

	bool Initialize(HINSTANCE);
	void Shutdown();
	bool Render();

	bool IsEscapePressed();
	bool IsUpPressed();
	bool IsDownPressed();
	bool IsLeftPressed();
	bool IsRightPressed();
	bool IsQPressed();
	bool IsWPressed();
	bool IsAPressed();
	bool IsSPressed();
	bool IsEPressed();
	bool IsRPressed();
	bool IsDPressed();
	bool IsFPressed();
	bool IsLeftClickPressed();
	bool IsRightClickPressed();
	bool IsSpacePressed();
	bool IsKeyPressed(unsigned int keycode);

	void GetMouseLocation(int&, int&);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;

};
static EventClass* EventInstance = 0;