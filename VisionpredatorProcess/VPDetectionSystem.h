#pragma once
#include "System.h"
#include "EventSubscriber.h"
#include "VisPredComponents.h"
class VPDetectionSystem :public System, public EventSubscriber, public ITriggerable,public IStartable
{
public:
	VPDetectionSystem(std::shared_ptr<SceneManager>scenemanger);
	~VPDetectionSystem() = default;


	void EntityEnter(std::shared_ptr<Entity> detected, std::shared_ptr<Entity> detector);
	void EntityExit(std::shared_ptr<Entity> detected, std::shared_ptr<Entity> detector);

	// ITriggerable을(를) 통해 상속됨
	void EnterTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second) override;
	void ExitTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second) override;

	// IStartable을(를) 통해 상속됨
	void Initialize() override;
	void Start(uint32_t gameObjectId) override;
	void Finish(uint32_t gameObjectId) override;
	void Finalize() override;
	void OnInterectionChanged(std::any entityID);
	void OnDetectiveMode(std::any entityID);
};

