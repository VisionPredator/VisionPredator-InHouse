#pragma once
#include "resource.h"

class TimeManager;
class SystemManager;
class EntityManager;
class VPEngine
{
public:
	HWND m_hWnd = nullptr;
	VPEngine(HINSTANCE hInstance, std::string title, int width, int height);
	~VPEngine();
	void Loop();

protected:
	virtual void Update();
	virtual void Render();
	TimeManager* m_TimeManager;
	SystemManager* m_SystemManager;
	EntityManager* m_EntityManager;
	float m_DeltaTime;
};

