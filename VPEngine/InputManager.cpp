#include "pch.h"
#include "InputManager.h"

InputManager* InputManager::instance = nullptr;


void InputManager::Initialize()
{
	for (int i = 0; i < (int)KEY::END; ++i) {
		m_KeyInputInfos.push_back(KeyInputInfo{ KEY_STATE::NONE, false });
	}
}

bool InputManager::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	// 마우스 커서의 위치 지정에 사용될 화면 크기를 설정합니다.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Direct Input 인터페이스를 초기화 합니다.
	HRESULT result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
		return false;

	// 키보드의 Direct Input 인터페이스를 생성합니다
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
		return false;

	// 데이터 형식을 설정하십시오. 이 경우 키보드이므로 사전 정의 된 데이터 형식을 사용할 수 있습니다.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
		return false;

	// 다른 프로그램과 공유하지 않도록 키보드의 협조 수준을 설정합니다
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
		return false;

	// 키보드를 할당받는다
	result = m_keyboard->Acquire();
	if (FAILED(result))
		return false;

	// 마우스 Direct Input 인터페이스를 생성합니다.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
		return false;

	// 미리 정의 된 마우스 데이터 형식을 사용하여 마우스의 데이터 형식을 설정합니다.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
		return false;

	// 다른 프로그램과 공유 할 수 있도록 마우스의 협력 수준을 설정합니다.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
		return false;

	// 마우스를 할당받는다
	result = m_mouse->Acquire();
	if (FAILED(result))
		return false;

	return true;
}

bool InputManager::Frame()
{
	// 키보드의 현재 상태를 읽는다.
	if (!ReadKeyboard())
	{
		return false;
	}

	// 마우스의 현재 상태를 읽는다.
	if (!ReadMouse())
	{
		return false;
	}

	// 키보드와 마우스의 변경상태를 처리합니다.
	ProcessInput();

	return true;
}



void InputManager::ProcessInput()
{
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;
	//프레임 동안 마우스 위치의 변경을 기반으로 마우스 커서의 위치를 업데이트한다.
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }
	//마우스 위치가 화면 너비 또는 높이를 초과하지 않는지 확인한다.
	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
}


InputManager::InputManager()
{
	EventManager::GetInstance().Subscribe("OnSetKeyState", CreateSubscriber(&InputManager::OnSetKeyState));

}

void InputManager::OnSetKeyState(std::any data)
//int vkCode, KEY_STATE state)
{
	auto [vkCode, state] = std::any_cast<std::pair<int, KEY_STATE>>(data);

	auto it = std::find(std::begin(arrayVK), std::end(arrayVK), vkCode);
	if (it != std::end(arrayVK))
	{
		size_t index = std::distance(std::begin(arrayVK), it);
		auto& keyInfo = m_KeyInputInfos[index];

		keyInfo.KeyState = state;
		keyInfo.IsPushed = (state == KEY_STATE::DOWN) || (state == KEY_STATE::HOLD);

		if (keyInfo.KeyState == KEY_STATE::DOWN && keyInfo.IsPushed)
		{
			// 1프레임 동안만 DOWN 유지 이후 HOLD로 변경
			EventManager::GetInstance().ScheduleEvent("OnSetKeyState", std::make_pair(vkCode, KEY_STATE::HOLD), 0.00001f);
		}
		else if (keyInfo.KeyState == KEY_STATE::UP && !keyInfo.IsPushed)
		{
			// 1프레임 동안만 UP 유지 이후 NONE으로 변경
			EventManager::GetInstance().ScheduleEvent("OnSetKeyState", std::make_pair(vkCode, KEY_STATE::NONE), 0.00001f);
		}
	}
}
void InputManager::Update()
{
	if (HWND hWnd = GetFocus(); hWnd != nullptr) // 화면이 포커스 되어있을 때만 인풋값을 받는다.
	{
		// 마우스 위치 저장하는 함수 포함하기
		POINT curPos;
		if (GetCursorPos(&curPos) && ScreenToClient(hWnd, &curPos)) // GetCursorPos 및 ScreenToClient 성공 여부 확인
		{
			auto [curX, curY] = std::pair{ static_cast<float>(curPos.x), static_cast<float>(curPos.y) };
			m_DeltaCurPos = { curX - m_CurPos.x, curY - m_CurPos.y };
			m_CurPos = { curX, curY };
		}
	}
}

bool InputManager::GetKey(KEY key)
{
	return m_KeyInputInfos[(int)key].KeyState == KEY_STATE::HOLD;
}

bool InputManager::GetKeyUp(KEY key)
{
	return m_KeyInputInfos[(int)key].KeyState == KEY_STATE::UP;

}

bool InputManager::GetKeyDown(KEY key)
{
	return  m_KeyInputInfos[(int)key].KeyState == KEY_STATE::DOWN;
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

bool InputManager::ReadKeyboard()
{
	// 키보드 디바이스를 얻는다.
	HRESULT result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// 키보드가 포커스를 잃었거나 획득되지 않은 경우 컨트롤을 다시 가져 온다
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
	// 마우스 디바이스를 얻는다.
	HRESULT result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// 마우스가 포커스를 잃었거나 획득되지 않은 경우 컨트롤을 다시 가져 온다
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

