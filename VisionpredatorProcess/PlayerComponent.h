#pragma once
#include "Component.h"
#include "VisPredStructs.h"
struct PlayerComponent :
	public Component
{
	PlayerComponent();
	VP_JSONBODY(PlayerComponent
		,HandName, VPHandName, CameraPosName, CameraName, FirePosName, LongswordName
		, MaxHP, HP, Sencitive, WalkSpeed, RunSpeed, SlideDuration, SearchDistance, StaticFriction, JumpForce, AirControlPercent, GravityPower, RecoilProgress, VPGageCoolTime, NonDamageTime, TransformationTime)

	std::string HandName{};
	std::string VPHandName{};
	std::string CameraPosName{};
	std::string CameraName{};
	std::string FirePosName;
	std::string LongswordName{};
	std::weak_ptr<Entity> HandEntity{};
	std::weak_ptr<Entity> VPHandEntity{};
	std::weak_ptr<Entity> CameraEntity{};
	std::weak_ptr<Entity> CameraPosEntity{};
	std::weak_ptr<Entity> FirePosEntity{};
	std::weak_ptr<Entity> LongswordEntity{};
	uint32_t MaxHP{};
	uint32_t HP{};
	VisPred::Game::PlayerFSM CurrentFSM = VisPred::Game::PlayerFSM::IDLE;
	float Height{};
	float Radius{};
	float Sencitive = 1.f;
	float WalkSpeed{};
	float RunSpeed{};
	//float SlideDisTance{};
	float StaticFriction{};
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
	bool IsAttacking{};
	float CamTransDuration = 0.07f;
	float CamTransProgress = 0.f;
	VPMath::Vector3 DefalutCameraPos{};
	float SlideDuration = 0.5f;
    float SlideProgress{};
    VPMath::Vector3 SlideDir{};
	float SearchDistance{};
	uint32_t SearchedItemID{};
	uint32_t PreSearchedItemID{};
	uint32_t GunEntityID{};
	uint32_t ThrowingGunEntityID{};
    bool HasGun=false;
    float GunprogressTime{};
    bool ReadyToShoot{};
	bool IsGunRecoiling{};
	float RecoilProgress{};

	bool IsVPMode{};
	bool NonDamageMode =false;
	bool IsTransformationing{};
	float VPGageProgress{};
	float VPGageCoolTime{};
	bool ReadyToTransform{};
	float TransformationProgress{};
	float TransformationTime =1;
	bool IsArmChanged{};
	float NonDamageProgress{};
	float NonDamageTime =1;
	float MaxNonDamageTime{};
	VPMath::Quaternion GunRecoilEndQuat{};
	VPMath::Quaternion GunRecoilStartQuat{};
};

