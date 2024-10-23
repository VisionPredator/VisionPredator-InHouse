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
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
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
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
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

void InputManager::CalculateMouseDelta()
{
	m_mouseDeltaX = m_mouseState.lX;
	m_mouseDeltaY = m_mouseState.lY;
}
