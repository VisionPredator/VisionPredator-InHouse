#pragma once
#include "System.h"
class AnimationSystem :
    public System, public EventSubscriber, public IUpdatable, public IFixedUpdatable, public IRenderable
{
public:
	AnimationSystem(SceneManager* sceneManager);
	~AnimationSystem() = default;

	void OnAddedComponent(std::any data);
	void OnReleasedComponent(std::any data);
	// IUpdatable��(��) ���� ��ӵ�
	void Update(float deltaTime) override;

	// IFixedUpdatable��(��) ���� ��ӵ�
	void FixedUpdate(float deltaTime) override;

	// IRenderable��(��) ���� ��ӵ�
	void RenderUpdate(float deltaTime) override;
};

