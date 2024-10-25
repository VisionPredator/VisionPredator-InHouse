#pragma once
#include "System.h"
#include "EventSubscriber.h"
class InterectiveSystem:public System,public EventSubscriber
{
public:
	InterectiveSystem(std::shared_ptr<SceneManager> scenemanager);
	~InterectiveSystem() = default;

	void OnSearched(std::any searchedEntityID);
	void OnUnSearched(std::any searchedEntityID);
	void OnInterective(std::any interective_interector);
	
};

