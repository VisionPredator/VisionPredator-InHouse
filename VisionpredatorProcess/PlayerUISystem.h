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
	void UpdateHP();
	void OnUpdateHP(std::any none);
	void UpdateVPState();
	void OnUpdateVPState(std::any none = {});
	void UpdateAim();
	void UpdateWeaponUI();
	void OnUpdateWeaponUI(std::any data = {});
	void SettingPlayerUI( float deltatime);
	void UpdateFadeUI();
	void UpdateHitUI(float deltatime);
	double Fade_in_out_Percent(float progress, float totalTime);
	double Fade_in_Percent(float progress, float totalTime);
	double Fade_out_Percent(float progress, float totalTime);

	void OnUpdateSearchUI(std::any null);
	void OnResetInterectionUI(std::any null);
	void OnHideBullet(std::any null = {});
	bool ResetInterectionUI(std::shared_ptr<Entity> identityComp);
	void UpdateInterectionUI(PlayerUIComponent& identityComp);
	bool InterectingGun(std::shared_ptr<Entity> weaponentity, Entity* selectedentity);
	bool InterectingObject(std::shared_ptr<Entity> weaponentity, Entity* selectedentity);
	void OnDamaged(std::any entity_Damage);
	// Event
	void OnShoot(std::any data);
	std::weak_ptr<Entity> m_PlayerUI;
	
};
