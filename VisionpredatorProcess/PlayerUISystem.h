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
	void Initialize() override;
	void Start(uint32_t gameObjectId) override {};
	void Finish(uint32_t gameObjectId) override {};
	void Finalize() override ;

	void UpdateHP(IdentityComponent& identityComp);
	void UpdateVPState(IdentityComponent& identityComp);
	void UpdateAim(IdentityComponent& identityComp);
	void UpdateWeaponUI(IdentityComponent& identityComp);

	void UpdateInterectionUI(IdentityComponent& identityComp);
	bool InterectUIReset(IdentityComponent& identityComp);
	bool InterectingGun(IdentityComponent& identityComp,Entity* selectedentity);
	bool InterectingDoor(IdentityComponent& identityComp,Entity* selectedentity);
	bool InterectingCloset(IdentityComponent& identityComp,Entity* selectedentity);

	// Event
	void OnGunShoot(std::any data);
	 PlayerComponent *m_PlayerComp{};
	
};
