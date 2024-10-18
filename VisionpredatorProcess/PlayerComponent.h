#pragma once
#include "Component.h"
#include "VisPredStructs.h"
struct PlayerComponent :
	public Component
{
	PlayerComponent();
	VP_JSONBODY(PlayerComponent,RecoilMode
		,HandName, CameraPosName, CameraName, FirePosName, LongswordName
		, MaxHP, HP, Sencitive, WalkSpeed, RunSpeed, SlideDuration, StaticFriction, DynamicFriction, JumpForce, AirControlPercent, GravityPower, RecoilProgress, RecoilReturnTime)

	VisPred::Game::GunRecoilMode RecoilMode{};
	std::string HandName{};
	std::string CameraPosName{};
	std::string CameraName{};
	std::string FirePosName;
	std::string LongswordName{};
	std::weak_ptr<Entity> HandEntity{};
	std::weak_ptr<Entity> CameraEntity{};
	std::weak_ptr<Entity> CameraPosEntity{};
	std::weak_ptr<Entity> FirePosEntity{};
	std::weak_ptr<Entity> LongswordEntity{};
	uint32_t MaxHP{};
	uint32_t HP{};
	VisPred::Game::EFSM CurrentFSM = VisPred::Game::EFSM::IDLE;
	float Height{};
	float Radius{};
	float Sencitive = 1.f;
	float WalkSpeed{};
	float RunSpeed{};
	//float SlideDisTance{};
	float StaticFriction{};
	float DynamicFriction{};
	float JumpForce{};
	float AirControlPercent{};
	float GravityPower = 1.f;
	/// <summary>
	/// 게임로직을 위한 변수들
	/// </summary>
	VPMath::Vector3 DownCameraPos{};
	VPMath::Vector3 SitCameraPos{};
	VPMath::Vector3 SlideCameraPos{};
	float SitHeight{};
	float SitHeightDiff{};
	float SlideHeight{};
	float SlideHeightDiff{};

	float CamTransDuration = 0.07f;
	float CamTransProgress = 0.f;
	VPMath::Vector3 DefalutCameraPos{};
	float SlideDuration = 0.5f;
    float SlideProgress{};
    VPMath::Vector3 SlideDir{};

	uint32_t SearchedItemID{};
	uint32_t PreSearchedItemID{};
	uint32_t GunEntityID{};
	uint32_t ThrowingGunEntityID{};
    VisPred::Game::GunType ShootType{};
    bool HasGun=false;
    float GunprogressTime{};
    bool ReadyToShoot{};
	bool IsRotated=false;
	bool PlayerCamereShake{};
	bool IsGunRecoiling{};
	bool IsEndReocilReturn{};
	float RecoilProgress{};
	float RecoilReturnTime{};
	VPMath::Quaternion GunRecoilEndQuat{};
	VPMath::Quaternion GunRecoilStartQuat{};
};

