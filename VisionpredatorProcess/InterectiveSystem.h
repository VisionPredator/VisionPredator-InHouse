#pragma once
#include "System.h"
#include "EventSubscriber.h"
class InterectiveSystem:public System,public EventSubscriber,public IPhysicable
{
public:
	InterectiveSystem(std::shared_ptr<SceneManager> scenemanager);
	~InterectiveSystem() = default;

	void OnSearched(std::any searchedEntityID);
	void OnUnSearched(std::any searchedEntityID);
	void OnInterective(std::any interective_interector);
	void OnInterected(std::any interective_interector);


	// IPhysicable을(를) 통해 상속됨
	void PhysicsUpdate(float deltaTime) override {};

	void PhysicsLateUpdate(float deltaTime) override {};

};

