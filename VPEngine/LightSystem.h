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


	// IRenderable��(��) ���� ��ӵ�
	void RenderUpdate(float deltaTime) override;
};

