#include "pch.h"
#include "InputManager.h"
namespace VisPred
{

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

		for (int i = 0; i < (int)KEY::END; ++i) 
		{
			if (arrayVK[i] == vkCode) 
			{
				m_KeyInputInfos[i].KeyState = state;
				m_KeyInputInfos[i].IsPushed = ((state == KEY_STATE::DOWN) || (state == KEY_STATE::HOLD));
				if (m_KeyInputInfos[i].KeyState==KEY_STATE::DOWN&& m_KeyInputInfos[i].IsPushed==true)
				{
					//1frame 동안만 DOWN 유지 이후 HOLD로 변경!
					std::pair<int, KEY_STATE> newdata = { vkCode ,KEY_STATE::HOLD };		
					EventManager::GetInstance().ScheduleEvent("OnSetKeyState", newdata,(float)0.00001);
				}
				else if(m_KeyInputInfos[i].KeyState == KEY_STATE::UP&& m_KeyInputInfos[i].IsPushed == false)
				{
					//1frame 동안만 UP 유지 이후 NONE로 변경!
					std::pair<int, KEY_STATE> newdata = { vkCode ,KEY_STATE::NONE };
					EventManager::GetInstance().ScheduleEvent("OnSetKeyState", newdata, (float)0.00001);
				}
				break;
			}
		}
	}

	void InputManager::Update()
	{
		HWND hWnd = GetFocus();		///화면이 포커스 되어있을 때만 인풋값을 받는다.
		if (hWnd != nullptr)
		{
			///마우스 위치 저장하는 함수 포함하기
			POINT curPos = {};
			GetCursorPos(&curPos);
			ScreenToClient(hWnd, &curPos);
			m_CurPos = { (float)curPos.x,(float)curPos.y };

			///Input관련 
			for (int i = 0; i < (int)KEY::END; ++i)
				if (m_KeyInputInfos[i].KeyState == KEY_STATE::DOWN)	//키가 눌림
					m_KeyInputInfos[i].KeyState = KEY_STATE::HOLD;
				else if ((m_KeyInputInfos[i].KeyState == KEY_STATE::UP))
					m_KeyInputInfos[i].KeyState = KEY_STATE::NONE;

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

}