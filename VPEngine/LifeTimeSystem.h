#pragma once
#include "System.h"
class LifeTimeSystem:public System,public IFixedUpdatable
{
public: 
	LifeTimeSystem(std::shared_ptr<SceneManager> sceneManager);
	~LifeTimeSystem() override = default;


	// IFixedUpdatable��(��) ���� ��ӵ�
	void FixedUpdate(float deltaTime) override;

};

