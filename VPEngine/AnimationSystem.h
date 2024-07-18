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


	// IRenderable��(��) ���� ��ӵ�
	void RenderUpdate(float deltaTime) override;
};

