#pragma once
#include "System.h"
class AnimationSystem :
    public System, public EventSubscriber, public IRenderable
{
public:
	AnimationSystem(SceneManager* sceneManager);
	~AnimationSystem() = default;

	void OnAddedComponent(std::any data);
	void OnReleasedComponent(std::any data);


	// IRenderable을(를) 통해 상속됨
	void RenderUpdate(float deltaTime) override;
};

