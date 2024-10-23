#include "pch.h"
#include "InputManager.h"
#include <iostream>

InputManager* InputManager::instance = nullptr;





InputManager::InputManager()
{

}



bool InputManager::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	// 마우스 커서의 위치 지정에 사용될 화면 크기를 설정합니다.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

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
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
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
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
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

bool InputManager::Update()
{
	//이전 키 설정을 저장하여 Key Down/Up/Hold 를 구별하는데 쓰입니다.
	CopyKeyStateToPrevious();
	CopyMouseStateToPrevious();

	// 키보드의 현재 상태를 읽는다.
	if (!ReadKeyboard())
		return false;
	// 마우스의 현재 상태를 읽는다.
	if (!ReadMouse())
		return false;
	//마우스의 변경상태를 처리합니다.
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
	CalculateMouseDelta();

	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;
	//프레임 동안 마우스 위치의 변경을 기반으로 마우스 커서의 위치를 업데이트한다.
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }
	//마우스 위치가 화면 너비 또는 높이를 초과하지 않는지 확인한다.
	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
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

void InputManager::CalculateMouseDelta()
{
	m_mouseDeltaX = m_mouseState.lX;
	m_mouseDeltaY = m_mouseState.lY;
}
