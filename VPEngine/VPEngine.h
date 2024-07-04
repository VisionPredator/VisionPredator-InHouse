#pragma once
#include "resource.h"
#include "../VPGraphics/IGraphics.h"



class TimeManager;
class SystemManager;
class SceneManager;

class VPEngine
{
public:
	HWND m_hWnd = nullptr;
	VPEngine(HINSTANCE hInstance, std::string title, int width, int height);
	~VPEngine();
	void Loop();

protected:
	void Addsystem();
	virtual void Update();
	virtual void Render();
	virtual void EndRender();
	TimeManager* m_TimeManager;
	SystemManager* m_SystemManager;
	SceneManager* m_SceneManager;
	float m_DeltaTime;


	Graphics::Interface* m_Graphics;

};

