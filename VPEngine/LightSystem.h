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
	// IUpdatable��(��) ���� ��ӵ�
	void Update(float deltaTime) override;

	// IFixedUpdatable��(��) ���� ��ӵ�
	void FixedUpdate(float deltaTime) override;

	// IRenderable��(��) ���� ��ӵ�
	void RenderUpdate(float deltaTime) override;
};

