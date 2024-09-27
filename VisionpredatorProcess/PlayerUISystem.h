#pragma once
#include "System.h"
#include "VisPredComponents.h"
#include "EventSubscriber.h"

class PlayerUISystem
	: public System
	, public IUpdatable
	, public IStartable
	, public EventSubscriber
{
public:
	PlayerUISystem(const std::shared_ptr<SceneManager>& sceneManager);
	~PlayerUISystem() override = default;

	void Update(float deltaTime) override;
	void Initialize() override {};
	void Start(uint32_t gameObjectId) override {};
	void Finish(uint32_t gameObjectId) override {};
	void Finalize() override {};

	void UpdateHP(const PlayerComponent& playerComponent);
	void UpdateVPState(const PlayerComponent& playerComponent);
	void UpdateAim(const PlayerComponent& playerComponent);
	void UpdateWeapon(const PlayerComponent& playerComponent);

	// Event
	void OnGunShoot(std::any data);
};
