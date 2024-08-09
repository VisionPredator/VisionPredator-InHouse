#pragma once
#include "resource.h"
#include "../VPGraphics/IGraphics.h"
#include "EventSubscriber.h"
namespace Physic
{
class IPhysx;
}

class TimeManager;
class SystemManager;
class SceneManager;
class EventSubscriber;

class VPEngine:public EventSubscriber
{
public:
	HINSTANCE m_hinstance=nullptr;

	HWND m_hWnd = nullptr;
	VPEngine(HINSTANCE hInstance, std::string title, int width, int height);
	~VPEngine();
	void Loop();
	int m_ScreenWidth {};
	int m_ScreenHeight{};
	static bool isResize;
protected:
	void Addsystem();
	virtual void Update();
	virtual void Render();
	virtual void EndRender();
	void OnAddSystemLater(std::any);
	TimeManager* m_TimeManager;
	std::shared_ptr<SystemManager> m_SystemManager;
	std::shared_ptr<SceneManager> m_SceneManager;
	
	float m_DeltaTime;

	Physic::IPhysx* m_PhysicEngine;
	Graphics::Interface* m_Graphics;


};

