#pragma once
#include "Component.h"
#include "VisPredStructs.h"
struct PlayerComponent :
	public Component
{
	VP_JSONBODY(PlayerComponent
		,HandName, VPHandName, CameraPosName, CameraName, FirePosName, LongswordName
		, MaxHP, HP, Sencitive, WalkSpeed, RunSpeed, VPRunSpeed, SlideDuration, DashDuration, DashMultiple, SlideMultiple, SearchDistance, StaticFriction, JumpForce, AirControlPercent, GravityPower, RecoilProgress, VPGageCoolTime, NonDamageTime, TransformationTime)

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
	std::weak_ptr<Entity> AutoPickEntity{};
	int MaxHP{110};
	int HP{110};
	VisPred::Game::PlayerFSM CurrentFSM = VisPred::Game::PlayerFSM::IDLE;
	VisPred::Game::PlayerFSM PreFSM = VisPred::Game::PlayerFSM::IDLE;
	float Height{};
	float Radius{};
	float Sencitive = 1.f;
	float WalkSpeed{};
	float RunSpeed{};
	float VPRunSpeed{};
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
	float DashDuration = 0.5f;
	float SlideMultiple = 1.5f;
	float DashMultiple = 3.f;
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
	bool IsSearchable = true;
	bool IsVPMode=true;
	bool NonDamageMode =false;
	bool GodMode =false;
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

	float DieProgress{};
	float DieTime=3;
	VPMath::Quaternion GunRecoilEndQuat{};
	VPMath::Quaternion GunRecoilStartQuat{};
};

