#include "pch.h"
#include "InputManager.h"
#include <iostream>

InputManager* InputManager::instance = nullptr;





InputManager::InputManager()
{

}



bool InputManager::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	// ���콺 Ŀ���� ��ġ ������ ���� ȭ�� ũ�⸦ �����մϴ�.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Direct Input �������̽��� �ʱ�ȭ �մϴ�.
	HRESULT result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Ű������ Direct Input �������̽��� �����մϴ�
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
		return false;

	// ������ ������ �����Ͻʽÿ�. �� ��� Ű�����̹Ƿ� ���� ���ǵ� ������ ������ ����� �� �ֽ��ϴ�.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
		return false;

	// �ٸ� ���α׷��� �������� �ʵ��� Ű������ ���� ������ �����մϴ�
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
		return false;

	// Ű���带 �Ҵ�޴´�
	result = m_keyboard->Acquire();
	if (FAILED(result))
		return false;

	// ���콺 Direct Input �������̽��� �����մϴ�.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
		return false;

	// �̸� ���� �� ���콺 ������ ������ ����Ͽ� ���콺�� ������ ������ �����մϴ�.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
		return false;

	// �ٸ� ���α׷��� ���� �� �� �ֵ��� ���콺�� ���� ������ �����մϴ�.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// ���콺�� �Ҵ�޴´�
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool InputManager::Update()
{
	//���� Ű ������ �����Ͽ� Key Down/Up/Hold �� �����ϴµ� ���Դϴ�.
	CopyKeyStateToPrevious();
	CopyMouseStateToPrevious();

	// Ű������ ���� ���¸� �д´�.
	if (!ReadKeyboard())
		return false;
	// ���콺�� ���� ���¸� �д´�.
	if (!ReadMouse())
		return false;
	//���콺�� ������¸� ó���մϴ�.
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
	CalculateMouseDelta();

	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;
	//������ ���� ���콺 ��ġ�� ������ ������� ���콺 Ŀ���� ��ġ�� ������Ʈ�Ѵ�.
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }
	//���콺 ��ġ�� ȭ�� �ʺ� �Ǵ� ���̸� �ʰ����� �ʴ��� Ȯ���Ѵ�.
	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
}

void InputManager::CopyKeyStateToPrevious()
{
	memcpy(m_previousKeyboardState, m_keyboardState, sizeof(m_keyboardState));
}

void InputManager::CopyMouseStateToPrevious()
{
	m_previousMouseState = m_mouseState;
}

bool InputManager::GetKeyDown(KEYBOARDKEY inputkey) 
{
	int keyCode = static_cast<int>(inputkey);
	bool result = (m_keyboardState[keyCode] & 0x80) && !(m_previousKeyboardState[keyCode] & 0x80);
	if (result)
	{
		std::cout << "GetKeyDown";

	}
	return result;
}

bool InputManager::GetKeyUp(KEYBOARDKEY inputkey) 
{
	int keyCode = static_cast<int>(inputkey);
	bool result = !(m_keyboardState[keyCode] & 0x80) && (m_previousKeyboardState[keyCode] & 0x80);
	if (result)
	{
		std::cout << "GetKeyUp";

	}
	return result;
}

bool InputManager::GetKey(KEYBOARDKEY inputkey) 
{
	int keyCode = static_cast<int>(inputkey);
	bool result = (m_keyboardState[keyCode] & 0x80) && (m_previousKeyboardState[keyCode] & 0x80);
	if (result)
	{
		std::cout << "GetKeyHold";
	}
	return result;
}

bool InputManager::GetKeyDown(MOUSEKEY inputkey)
{
	// ���콺 ��ư�� ���� ���� �ְ�, ���� �����ӿ����� ���� ���� �ʴٸ� true ��ȯ
	bool result = (m_mouseState.rgbButtons[static_cast<int>(inputkey)] & 0x80) &&
		!(m_previousMouseState.rgbButtons[static_cast<int>(inputkey)] & 0x80);
	if (result)
	{
		std::cout << "GetKeyDown";
	}
	return result;
}

bool InputManager::GetKeyUp(MOUSEKEY inputkey)
{
	// ���콺 ��ư�� ���� ���� ���� �ʰ�, ���� �����ӿ����� ���� �־��ٸ� true ��ȯ
	bool result = !(m_mouseState.rgbButtons[static_cast<int>(inputkey)] & 0x80) &&
		(m_previousMouseState.rgbButtons[static_cast<int>(inputkey)] & 0x80);
	if (result)
	{
		std::cout << "GetKeyUp";
	}
	return result;
}

bool InputManager::GetKey(MOUSEKEY inputkey)
{
	// ���콺 ��ư�� ����� ���� ������ ��ο��� ���� �ִٸ� true ��ȯ
	bool result = (m_mouseState.rgbButtons[static_cast<int>(inputkey)] & 0x80) &&
		(m_previousMouseState.rgbButtons[static_cast<int>(inputkey)] & 0x80);
	if (result)
	{
		std::cout << "GetKeyHold";
	}
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
bool InputManager::ReadKeyboard()
{
	// Ű���� ����̽��� ��´�.
	HRESULT result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// Ű���尡 ��Ŀ���� �Ҿ��ų� ȹ����� ���� ��� ��Ʈ���� �ٽ� ���� �´�
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputManager::ReadMouse()
{
	// ���콺 ����̽��� ��´�.
	HRESULT result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// ���콺�� ��Ŀ���� �Ҿ��ų� ȹ����� ���� ��� ��Ʈ���� �ٽ� ���� �´�
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void InputManager::CalculateMouseDelta()
{
	m_mouseDeltaX = m_mouseState.lX;
	m_mouseDeltaY = m_mouseState.lY;
}
