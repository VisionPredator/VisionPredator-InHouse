#pragma once
#include "System.h"
#include "EventSubscriber.h"

class LightSystem :
	public System, public EventSubscriber,public IRenderable
{
public:
	LightSystem(std::shared_ptr<SceneManager> entityManager);
	~LightSystem() = default;
	void OnAddedComponent(std::any data);
	void OnReleasedComponent(std::any data);


	// IRenderable을(를) 통해 상속됨
	void RenderUpdate(float deltaTime) override;
};

