#pragma once
#include "resource.h"

namespace VisPred
{
	class TimeManager;
	class SystemManager;
	class EntityManager;
	class GameProcess
	{
	public:
		HWND m_hWnd = nullptr;
		GameProcess(HINSTANCE hInstance, std::string title, int width, int height);
		~GameProcess();
		void Loop();

	protected:
		virtual void Update();
		virtual void Render();
		TimeManager* m_TimeManager;
		SystemManager* m_SystemManager;
		EntityManager* m_EntityManager;
		float m_DeltaTime;
	};

}