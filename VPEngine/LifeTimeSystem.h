#pragma once
#include "System.h"
class LifeTimeSystem:public System,public IFixedUpdatable
{
public: 
	LifeTimeSystem(std::shared_ptr<SceneManager> sceneManager);
	~LifeTimeSystem() override = default;


	// IFixedUpdatable을(를) 통해 상속됨
	void FixedUpdate(float deltaTime) override;

};

