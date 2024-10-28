#include "pch.h"
#include "InputManager.h"
#include <iostream>

InputManager* InputManager::instance = nullptr;





InputManager::InputManager()
{
	EventManager::GetInstance().Subscribe("OnResize", CreateSubscriber(&InputManager::OnResize));
	EventManager::GetInstance().Subscribe("OnClipMouse", CreateSubscriber(&InputManager::OnClipMouse));
}



bool InputManager::Initialize(HINSTANCE hinstance, HWND* hwnd)
{
	m_hwnd = hwnd;
	// 마우스 커서의 위치 지정에 사용될 화면 크기를 설정합니다.
	OnResize(hwnd);
	// Direct Input 인터페이스를 초기화 합니다.
	HRESULT result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	//if (FAILED(result))
	//{
	//	return false;
	//}
	bool returnbool = true;
	// 키보드의 Direct Input 인터페이스를 생성합니다
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		VP_ASSERT(false, "m_directInput->CreateDevice");
		
		returnbool = false;
	}


	// 데이터 형식을 설정하십시오. 이 경우 키보드이므로 사전 정의된 데이터 형식을 사용할 수 있습니다.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		VP_ASSERT(false, "m_keyboard->SetDataFormat");
		returnbool = false;
	}
	// 다른 프로그램과 공유하지 않도록 키보드의 협조 수준을 설정합니다
	result = m_keyboard->SetCooperativeLevel(*m_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		VP_ASSERT(false, "m_keyboard->SetCooperativeLevel");
		returnbool = false;
	}


	// 마우스 Direct Input 인터페이스를 생성합니다.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		VP_ASSERT(false, " m_directInput->CreateDevice");
		returnbool = false;
	}

	// 미리 정의 된 마우스 데이터 형식을 사용하여 마우스의 데이터 형식을 설정합니다.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		VP_ASSERT(false, " m_mouse->SetDataFormat");
		returnbool = false;
	}
	// 다른 프로그램과 공유 할 수 있도록 마우스의 협력 수준을 설정합니다.
	result = m_mouse->SetCooperativeLevel(*m_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		VP_ASSERT(false, " m_mouse->SetCooperativeLevel");
		returnbool = false;
	}

	// 키보드를 할당받는다
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		std::cout << "m_keyboard->Acquire()" << std::endl;
		returnbool = false;
	}
	// 마우스를 할당받는다
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		std::cout << "m_mouse->Acquire()"<<std::endl;
		returnbool = false;
	}

	return returnbool;
}
void InputManager::SetClipMode(bool IsWindowMode) {
	m_IsWindowMode = IsWindowMode;
	std::any data;
	OnClipMouse(data);
}
void InputManager::OnClipMouse(std::any hwnd)
{
	// Apply clipping if enabled
	if (m_IsWindowMode)
	{
		GetWindowRect(*m_hwnd, &m_clientRect);
		ClipCursor(&m_clientRect);
	}
	else
	{
		RECT clientRect{};
		GetClientRect(*m_hwnd, &m_clientRect);

		// Convert client coordinates to screen coordinates
		POINT topLeft = { m_clientRect.left, m_clientRect.top };
		POINT bottomRight = { m_clientRect.right, m_clientRect.bottom };
		ClientToScreen(*m_hwnd, &topLeft);
		ClientToScreen(*m_hwnd, &bottomRight);

		// Update clientRect with screen coordinates
		m_clientRect.left = topLeft.x;
		m_clientRect.top = topLeft.y;
		m_clientRect.right = bottomRight.x;
		m_clientRect.bottom = bottomRight.y;

		// Apply the clipping
		ClipCursor(&m_clientRect);
	}
}
bool InputManager::Update() {
	// Store previous key and mouse states
	CopyKeyStateToPrevious();
	CopyMouseStateToPrevious();
	// Read current keyboard state
	if (!ReadKeyboard()) return false;
	// Read current mouse state
	if (!ReadMouse()) return false;
	// Process mouse input
	ProcessMouseInput();

	return true;
}


void InputManager::Shutdown()
{
	// 마우스를 반환합니다.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// 키보드를 반환합니다.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// m_directInput 객체를 반환합니다.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}
}
void InputManager::ProcessMouseInput()
{

	if (m_hwnd)
	{
	// Get the current cursor position in screen coordinates
	GetCursorPos(&m_curPos);
		// Convert screen coordinates to client area coordinates
		ScreenToClient(*m_hwnd, &m_curPos);

		// Ensure the mouse position stays within the bounds of the client area using m_clientRect
		if (m_curPos.x < 0) m_curPos.x = 0;
		if (m_curPos.y < 0) m_curPos.y = 0;
		if (m_curPos.x > m_clientRect.right) m_curPos.x = m_clientRect.right;
		if (m_curPos.y > m_clientRect.bottom) m_curPos.y = m_clientRect.bottom;

		// Calculate the delta
		m_mouseDelta.x = m_mouseState.lX;
		m_mouseDelta.y = m_mouseState.lY;

	/*	m_mouseDelta.x = m_curPos.x - m_lastPos.x;
		m_mouseDelta.y = m_curPos.y - m_lastPos.y;*/

		// Update the last position for the next frame
		m_lastPos = m_curPos;
	}
}


void InputManager::OnResize(std::any hwnd)
{
	OnClipMouse(hwnd);
}
void InputManager::CopyKeyStateToPrevious()
{
	std::copy(std::begin(m_keyboardState), std::end(m_keyboardState), std::begin(m_previousKeyboardState));
}

void InputManager::CopyMouseStateToPrevious()
{
	m_previousMouseState = m_mouseState; // 이 경우는 동일합니다.
}


bool InputManager::GetKeyDown(KEYBOARDKEY inputkey)
{
	int keyCode = static_cast<int>(inputkey);
	bool result = (m_keyboardState[keyCode] & 0x80) && !(m_previousKeyboardState[keyCode] & 0x80);
#ifdef _DEBUG
	if (result)
	{
		//std::cout << "GetKeyDown" << std::endl;
	}
#endif
	return result;
}

// 동일한 방법으로 다른 GetKey* 메서드도 수정


bool InputManager::GetKeyUp(KEYBOARDKEY inputkey) 
{
	int keyCode = static_cast<int>(inputkey);
	bool result = !(m_keyboardState[keyCode] & 0x80) && (m_previousKeyboardState[keyCode] & 0x80);
#ifdef _DEBUG
	if (result)
	{
		//std::cout << "GetKeyUp";

	}
#endif
	return result;
}

bool InputManager::GetKey(KEYBOARDKEY inputkey) 
{
	int keyCode = static_cast<int>(inputkey);
	bool result = (m_keyboardState[keyCode] & 0x80) && (m_previousKeyboardState[keyCode] & 0x80);
#ifdef _DEBUG

#endif
	return result;
}

bool InputManager::GetKeyDown(MOUSEKEY inputkey)
{
	// 마우스 버튼이 현재 눌려 있고, 이전 프레임에서는 눌려 있지 않다면 true 반환
	bool result = (m_mouseState.rgbButtons[static_cast<int>(inputkey)] & 0x80) &&
		!(m_previousMouseState.rgbButtons[static_cast<int>(inputkey)] & 0x80);
#ifdef _DEBUG
	if (result)
	{
		//std::cout << "GetKeyDown";
	}
#endif
	return result;
}

bool InputManager::GetKeyUp(MOUSEKEY inputkey)
{
	// 마우스 버튼이 현재 눌려 있지 않고, 이전 프레임에서는 눌려 있었다면 true 반환
	bool result = !(m_mouseState.rgbButtons[static_cast<int>(inputkey)] & 0x80) &&
		(m_previousMouseState.rgbButtons[static_cast<int>(inputkey)] & 0x80);
#ifdef _DEBUG
	if (result)
	{
		//std::cout << "GetKeyUp";
	}
#endif
	return result;
}

bool InputManager::GetKey(MOUSEKEY inputkey)
{
	// 마우스 버튼이 현재와 이전 프레임 모두에서 눌려 있다면 true 반환
	bool result = (m_mouseState.rgbButtons[static_cast<int>(inputkey)] & 0x80) &&
		(m_previousMouseState.rgbButtons[static_cast<int>(inputkey)] & 0x80);
#ifdef _DEBUG

#endif
	return result;
}

bool InputManager::IsEscapePressed()
{
	// escape 키가 현재 눌려지고 있는지 bit값을 계산하여 확인한다.
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputManager::GetClipmode()
{
	return m_IsWindowMode;
}

bool InputManager::ReadKeyboard()
{
	HRESULT result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// 실패한 경우 로그 기록 (디버깅 용도로만 사용)
		//std::cerr << "Failed to get keyboard state. HRESULT: " << result << std::endl;

		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			result = m_keyboard->Acquire();
			if (FAILED(result))
			{
				//std::cerr << "Failed to acquire keyboard. HRESULT: " << result << std::endl;
				return false;
			}
		}
		else
			return false;
	}
	return true;
}

bool InputManager::ReadMouse()
{
	HRESULT result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// 실패한 경우 로그 기록 (디버깅 용도로만 사용)
		//std::cerr << "Failed to get mouse state. HRESULT: " << result << std::endl;

		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			result = m_mouse->Acquire();
			if (FAILED(result))
			{
				//std::cerr << "Failed to acquire mouse. HRESULT: " << result << std::endl;
				return false;
			}
		}
		else
			return false;
	}
	return true;
}

