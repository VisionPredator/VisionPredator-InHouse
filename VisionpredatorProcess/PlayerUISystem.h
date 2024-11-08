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
	void Start(uint32_t gameObjectId) override;
	void Finish(uint32_t gameObjectId) override {};
	void Finalize() override ;
	void UpdateHP(PlayerUIComponent& playerUI);
	void UpdateVPState(PlayerUIComponent& playerUI);
	void UpdateAim(PlayerUIComponent& playerUI);
	void UpdateWeaponUI(PlayerUIComponent& playerUI);
	void UpdatePlayerUI(PlayerUIComponent& playerUI, float deltatime);
	void UpdateFadeUI(PlayerUIComponent& playerUI);
	void UpdateHitUI(PlayerUIComponent& playerUI, float deltatime);
	double Fade_in_out_Percent(float progress, float totalTime);
	double Fade_in_Percent(float progress, float totalTime);
	double Fade_out_Percent(float progress, float totalTime);

	void OnUpdateSearchUI(std::any null);
	void OnResetInterectionUI(std::any null);
	bool ResetInterectionUI(std::shared_ptr<Entity> identityComp);
	void UpdateInterectionUI(PlayerUIComponent& identityComp);
	bool InterectingGun(std::shared_ptr<Entity> weaponentity, Entity* selectedentity);
	void OnDamaged(std::any entity_Damage);
	// Event
	void OnGunShoot(std::any data);
	 //PlayerComponent *m_PlayerComp{};
	
};
