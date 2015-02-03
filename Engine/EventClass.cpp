#include "EventClass.h"
#include "WindowClass.h"
#include "GenericException.h"

EventClass::EventClass()
{
}

EventClass::~EventClass()
{
}

void EventClass::Initialize(HINSTANCE hinstance)
{
    HRESULT result;

    EventInstance = this;

    // Store the screen size which will be used for positioning the mouse cursor.
    m_screenWidth = WindowClass::getInstance()->getScreenWidth();
    m_screenHeight = WindowClass::getInstance()->getScreenHeight();

    // Initialize the location of the mouse on the screen.
    m_mouseX = 0;
    m_mouseY = 0;

    // Initialize the main direct input interface.
    result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, nullptr);
    if(FAILED(result))
    {
        throw GenericException("Failed to initialize the direct input interface");
    }

    // Initialize the direct input interface for the keyboard.
    result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, nullptr);
    if(FAILED(result))
    {
        throw GenericException("Failed to initialize the direct input interface for the keyboard");
    }

    // Set the data format.  In this case since it is a keyboard we can use the predefined data format.
    result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
    if(FAILED(result))
    {
        throw GenericException("Failed to set the keyboard data format");
    }

    // Set the cooperative level of the keyboard to not share with other programs.
    result = m_keyboard->SetCooperativeLevel(WindowClass::getInstance()->gethWnd(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
    if(FAILED(result))
    {
        throw GenericException("Failed to set the cooperation level of the keyboard");
    }

    // Now acquire the keyboard.
    result = m_keyboard->Acquire();
    if(FAILED(result))
    {
        throw GenericException("Failed to acquire the keyboard");
    }

    // Initialize the direct input interface for the mouse.
    result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, nullptr);
    if(FAILED(result))
    {
        throw GenericException("Failed to initialize the direct input interface for the mouse.");
    }

    // Set the data format for the mouse using the pre-defined mouse data format.
    result = m_mouse->SetDataFormat(&c_dfDIMouse);
    if(FAILED(result))
    {
        throw GenericException("Failed to set the data format of the mouse.");
    }

    // Set the cooperative level of the mouse to share with other programs.
    result = m_mouse->SetCooperativeLevel(WindowClass::getInstance()->gethWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if(FAILED(result))
    {
        throw GenericException("Failed to set cooperation level of the mouse.");
    }

    // Acquire the mouse.
    result = m_mouse->Acquire();
    if(FAILED(result))
    {
        throw GenericException("Failed to acquire the mouse.");
    }
}

void EventClass::Shutdown()
{
    // Release the mouse.
    if(m_mouse)
    {
        m_mouse->Unacquire();
        m_mouse->Release();
        m_mouse = 0;
    }

    // Release the keyboard.
    if(m_keyboard)
    {
        m_keyboard->Unacquire();
        m_keyboard->Release();
        m_keyboard = 0;
    }

    // Release the main interface to direct input.
    if(m_directInput)
    {
        m_directInput->Release();
        m_directInput = 0;
    }

    return;
}


void EventClass::Render()
{
    // Read the current state of the keyboard.
    ReadKeyboard();

    // Read the current state of the mouse.
    ReadMouse();

    // Process the changes in the mouse and keyboard.
    ProcessInput();
}


void EventClass::ReadKeyboard()
{
    HRESULT result;

    // Read the keyboard device.
    result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
    if(FAILED(result))
    {
        // If the keyboard lost focus or was not acquired then try to get control back.
        if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            result = m_keyboard->Acquire();
        }
    }
}


void EventClass::ReadMouse()
{
    HRESULT result;

    // Read the mouse device.
    result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
    if(FAILED(result))
    {
        // If the mouse lost focus or was not acquired then try to get control back.
        if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            result = m_mouse->Acquire();
        }
    }
}


void EventClass::ProcessInput()
{
    // Update the location of the mouse cursor based on the change of the mouse location during the frame.
    m_mouseX += m_mouseState.lX;
    m_mouseY += m_mouseState.lY;

    // Ensure the mouse location doesn't exceed the screen width or height.
    if(m_mouseX < 0)  { m_mouseX = 0; }
    if(m_mouseY < 0)  { m_mouseY = 0; }
    
    if(m_mouseX > m_screenWidth)  { m_mouseX = m_screenWidth; }
    if(m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
}


bool EventClass::IsEscapePressed()
{
    // Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
    if(m_keyboardState[DIK_ESCAPE] & 0x80)
    {
        return true;
    }

    return false;
}

bool EventClass::IsUpPressed()
{
    // Do a bitwise and on the keyboard state to check if the up key is currently being pressed.
    if(m_keyboardState[DIK_UPARROW] & 0x80)
    {
        return true;
    }

    return false;
}

bool EventClass::IsDownPressed()
{
    // Do a bitwise and on the keyboard state to check if the down key is currently being pressed.
    if(m_keyboardState[DIK_DOWNARROW] & 0x80)
    {
        return true;
    }

    return false;
}

bool EventClass::IsLeftPressed()
{
    // Do a bitwise and on the keyboard state to check if the left key is currently being pressed.
    if(m_keyboardState[DIK_LEFTARROW] & 0x80)
    {
        return true;
    }

    return false;
}

bool EventClass::IsRightPressed()
{
    // Do a bitwise and on the keyboard state to check if the right key is currently being pressed.
    if(m_keyboardState[DIK_RIGHTARROW] & 0x80)
    {
        return true;
    }

    return false;
}

bool EventClass::IsQPressed()
{
    // Do a bitwise and on the keyboard state to check if the right key is currently being pressed.
    if(m_keyboardState[DIK_Q] & 0x80)
    {
        return true;
    }

    return false;
}

bool EventClass::IsWPressed()
{
    // Do a bitwise and on the keyboard state to check if the right key is currently being pressed.
    if(m_keyboardState[DIK_W] & 0x80)
    {
        return true;
    }

    return false;
}

bool EventClass::IsAPressed()
{
    // Do a bitwise and on the keyboard state to check if the A key is currently being pressed.
    if(m_keyboardState[DIK_A] & 0x80)
    {
        return true;
    }

    return false;
}

bool EventClass::IsSPressed()
{
    // Do a bitwise and on the keyboard state to check if the S key is currently being pressed.
    if(m_keyboardState[DIK_S] & 0x80)
    {
        return true;
    }

    return false;
}

bool EventClass::IsEPressed()
{
    // Do a bitwise and on the keyboard state to check if the A key is currently being pressed.
    if(m_keyboardState[DIK_E] & 0x80)
    {
        return true;
    }

    return false;
}

bool EventClass::IsRPressed()
{
    // Do a bitwise and on the keyboard state to check if the S key is currently being pressed.
    if(m_keyboardState[DIK_R] & 0x80)
    {
        return true;
    }

    return false;
}

bool EventClass::IsDPressed()
{
    // Do a bitwise and on the keyboard state to check if the A key is currently being pressed.
    if(m_keyboardState[DIK_D] & 0x80)
    {
        return true;
    }

    return false;
}

bool EventClass::IsSpacePressed()
{
    // Do a bitwise and on the keyboard state to check if the A key is currently being pressed.
    if(m_keyboardState[DIK_SPACE] & 0x80)
    {
        return true;
    }

    return false;
}

bool EventClass::IsFPressed()
{
    // Do a bitwise and on the keyboard state to check if the S key is currently being pressed.
    if(m_keyboardState[DIK_F] & 0x80)
    {
        return true;
    }

    return false;
}

bool EventClass::IsLeftClickPressed()
{
    // Do a bitwise and on the keyboard state to check if the A key is currently being pressed.
    if(m_mouseState.rgbButtons[DIMOUSE_LEFTBUTTON] & 0x80)
    {
        return true;
    }

    return false;
}

bool EventClass::IsRightClickPressed()
{
    // Do a bitwise and on the keyboard state to check if the S key is currently being pressed.
    if(m_mouseState.rgbButtons[DIMOUSE_RIGHTBUTTON] & 0x80)
    {
        return true;
    }

    return false;
}

void EventClass::GetMouseLocation(int& mouseX, int& mouseY)
{
    mouseX = m_mouseX;
    mouseY = m_mouseY;
    return;
}

