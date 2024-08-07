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
	// ���콺 Ŀ���� ��ġ ������ ���� ȭ�� ũ�⸦ �����մϴ�.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Direct Input �������̽��� �ʱ�ȭ �մϴ�.
	HRESULT result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
		return false;

	// Ű������ Direct Input �������̽��� �����մϴ�
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
		return false;

	// ������ ������ �����Ͻʽÿ�. �� ��� Ű�����̹Ƿ� ���� ���� �� ������ ������ ����� �� �ֽ��ϴ�.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
		return false;

	// �ٸ� ���α׷��� �������� �ʵ��� Ű������ ���� ������ �����մϴ�
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
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
		return false;

	// ���콺�� �Ҵ�޴´�
	result = m_mouse->Acquire();
	if (FAILED(result))
		return false;

	return true;
}

bool InputManager::Frame()
{
	// Ű������ ���� ���¸� �д´�.
	if (!ReadKeyboard())
	{
		return false;
	}

	// ���콺�� ���� ���¸� �д´�.
	if (!ReadMouse())
	{
		return false;
	}

	// Ű����� ���콺�� ������¸� ó���մϴ�.
	ProcessInput();

	return true;
}



void InputManager::ProcessInput()
{
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;
	//������ ���� ���콺 ��ġ�� ������ ������� ���콺 Ŀ���� ��ġ�� ������Ʈ�Ѵ�.
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }
	//���콺 ��ġ�� ȭ�� �ʺ� �Ǵ� ���̸� �ʰ����� �ʴ��� Ȯ���Ѵ�.
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
			// 1������ ���ȸ� DOWN ���� ���� HOLD�� ����
			EventManager::GetInstance().ScheduleEvent("OnSetKeyState", std::make_pair(vkCode, KEY_STATE::HOLD), 0.00001f);
		}
		else if (keyInfo.KeyState == KEY_STATE::UP && !keyInfo.IsPushed)
		{
			// 1������ ���ȸ� UP ���� ���� NONE���� ����
			EventManager::GetInstance().ScheduleEvent("OnSetKeyState", std::make_pair(vkCode, KEY_STATE::NONE), 0.00001f);
		}
	}
}
void InputManager::Update()
{
	if (HWND hWnd = GetFocus(); hWnd != nullptr) // ȭ���� ��Ŀ�� �Ǿ����� ���� ��ǲ���� �޴´�.
	{
		// ���콺 ��ġ �����ϴ� �Լ� �����ϱ�
		POINT curPos;
		if (GetCursorPos(&curPos) && ScreenToClient(hWnd, &curPos)) // GetCursorPos �� ScreenToClient ���� ���� Ȯ��
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

