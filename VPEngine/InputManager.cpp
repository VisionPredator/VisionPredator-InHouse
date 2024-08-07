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

