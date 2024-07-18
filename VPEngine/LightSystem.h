#pragma once
#include "System.h"
#include "EventSubscriber.h"

class LightSystem :
	public System, public EventSubscriber, public IUpdatable, public IFixedUpdatable, public IRenderable
{
public:
	LightSystem(SceneManager* entityManager);
	~LightSystem() = default;
	void OnAddedComponent(std::any data);
	void OnReleasedComponent(std::any data);
	// IUpdatable을(를) 통해 상속됨
	void Update(float deltaTime) override;

	// IFixedUpdatable을(를) 통해 상속됨
	void FixedUpdate(float deltaTime) override;

	// IRenderable을(를) 통해 상속됨
	void RenderUpdate(float deltaTime) override;
};

