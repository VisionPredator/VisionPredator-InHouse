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
	// ���콺 Ŀ���� ��ġ ������ ���� ȭ�� ũ�⸦ �����մϴ�.
	OnResize(hwnd);
	// Direct Input �������̽��� �ʱ�ȭ �մϴ�.
	HRESULT result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	//if (FAILED(result))
	//{
	//	return false;
	//}
	bool returnbool = true;
	// Ű������ Direct Input �������̽��� �����մϴ�
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		VP_ASSERT(false, "m_directInput->CreateDevice");
		
		returnbool = false;
	}


	// ������ ������ �����Ͻʽÿ�. �� ��� Ű�����̹Ƿ� ���� ���ǵ� ������ ������ ����� �� �ֽ��ϴ�.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		VP_ASSERT(false, "m_keyboard->SetDataFormat");
		returnbool = false;
	}
	// �ٸ� ���α׷��� �������� �ʵ��� Ű������ ���� ������ �����մϴ�
	result = m_keyboard->SetCooperativeLevel(*m_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		VP_ASSERT(false, "m_keyboard->SetCooperativeLevel");
		returnbool = false;
	}


	// ���콺 Direct Input �������̽��� �����մϴ�.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		VP_ASSERT(false, " m_directInput->CreateDevice");
		returnbool = false;
	}

	// �̸� ���� �� ���콺 ������ ������ ����Ͽ� ���콺�� ������ ������ �����մϴ�.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		VP_ASSERT(false, " m_mouse->SetDataFormat");
		returnbool = false;
	}
	// �ٸ� ���α׷��� ���� �� �� �ֵ��� ���콺�� ���� ������ �����մϴ�.
	result = m_mouse->SetCooperativeLevel(*m_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		VP_ASSERT(false, " m_mouse->SetCooperativeLevel");
		returnbool = false;
	}

	// Ű���带 �Ҵ�޴´�
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		std::cout << "m_keyboard->Acquire()" << std::endl;
		returnbool = false;
	}
	// ���콺�� �Ҵ�޴´�
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
	// ���콺�� ��ȯ�մϴ�.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Ű���带 ��ȯ�մϴ�.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// m_directInput ��ü�� ��ȯ�մϴ�.
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
	m_previousMouseState = m_mouseState; // �� ���� �����մϴ�.
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

// ������ ������� �ٸ� GetKey* �޼��嵵 ����


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
	// ���콺 ��ư�� ���� ���� �ְ�, ���� �����ӿ����� ���� ���� �ʴٸ� true ��ȯ
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
	// ���콺 ��ư�� ���� ���� ���� �ʰ�, ���� �����ӿ����� ���� �־��ٸ� true ��ȯ
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
	// ���콺 ��ư�� ����� ���� ������ ��ο��� ���� �ִٸ� true ��ȯ
	bool result = (m_mouseState.rgbButtons[static_cast<int>(inputkey)] & 0x80) &&
		(m_previousMouseState.rgbButtons[static_cast<int>(inputkey)] & 0x80);
#ifdef _DEBUG

#endif
	return result;
}

bool InputManager::IsEscapePressed()
{
	// escape Ű�� ���� �������� �ִ��� bit���� ����Ͽ� Ȯ���Ѵ�.
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
		// ������ ��� �α� ��� (����� �뵵�θ� ���)
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
		// ������ ��� �α� ��� (����� �뵵�θ� ���)
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

