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
					//1frame ���ȸ� DOWN ���� ���� HOLD�� ����!
					std::pair<int, KEY_STATE> newdata = { vkCode ,KEY_STATE::HOLD };		
					EventManager::GetInstance().ScheduleEvent("OnSetKeyState", newdata,(float)0.00001);
				}
				else if(m_KeyInputInfos[i].KeyState == KEY_STATE::UP&& m_KeyInputInfos[i].IsPushed == false)
				{
					//1frame ���ȸ� UP ���� ���� NONE�� ����!
					std::pair<int, KEY_STATE> newdata = { vkCode ,KEY_STATE::NONE };
					EventManager::GetInstance().ScheduleEvent("OnSetKeyState", newdata, (float)0.00001);
				}
				break;
			}
		}
	}

	void InputManager::Update()
	{
		HWND hWnd = GetFocus();		///ȭ���� ��Ŀ�� �Ǿ����� ���� ��ǲ���� �޴´�.
		if (hWnd != nullptr)
		{
			///���콺 ��ġ �����ϴ� �Լ� �����ϱ�
			POINT curPos = {};
			GetCursorPos(&curPos);
			ScreenToClient(hWnd, &curPos);
			m_CurPos = { (float)curPos.x,(float)curPos.y };

			///Input���� 
			for (int i = 0; i < (int)KEY::END; ++i)
				if (m_KeyInputInfos[i].KeyState == KEY_STATE::DOWN)	//Ű�� ����
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