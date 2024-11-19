#pragma once
#include "resource.h"
#include "../VPGraphics/IGraphics.h"
#include "EventSubscriber.h"
#include "../SoundEngine/ISound.h"
#include "../SoundEngine/SoundEngine.h"

namespace Physic
{
	class IPhysx;
}
namespace Sound
{
	class ISound;
}

class TimeManager;
class SystemManager;
class SceneManager;
class TransformSystem;
class EventSubscriber;

class VPEngine :public EventSubscriber
{
public:
	HINSTANCE m_hinstance=nullptr;

	HWND m_hWnd = nullptr;
	VPEngine(HINSTANCE hInstance, std::string title, int width, int height);
	bool m_IsMenubar = true;;
	~VPEngine();
	void Loop();
	int m_ScreenWidth {};
	int m_ScreenHeight{};
	static bool isResize;
	static bool isFullScreen;
protected:
	void Addsystem();
	virtual void Update();
	virtual void Render();
	virtual void RenderUpdate();
	virtual void BeginRender();
	virtual bool ImguiBeginRender();
	virtual void ImguiEndRender();
	virtual void EndRender();
	void OnAddSystemLater(std::any);
	TimeManager* m_TimeManager;
	std::shared_ptr<SystemManager> m_SystemManager;
	std::shared_ptr<SceneManager> m_SceneManager;
	float m_DeltaTime;
	float m_TotalGameTime;
	Physic::IPhysx* m_PhysicEngine;
	Graphics::Interface* m_Graphics;
	std::shared_ptr<Sound::ISound> m_SoundEngine;
	TransformSystem* m_TransformSystem;
};

