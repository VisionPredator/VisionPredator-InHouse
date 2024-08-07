#include "pch.h"
#include "InputManager.h"

	 InputManager* InputManager::instance = nullptr;


	void InputManager::Initialize()
	{
		for (int i = 0; i < (int)KEY::END; ++i) {
			m_KeyInputInfos.push_back(KeyInputInfo{ KEY_STATE::NONE, false });
		}
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

