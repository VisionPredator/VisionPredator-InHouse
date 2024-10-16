#pragma once
#include "Component.h"
#include "VisPredStructs.h"
struct PlayerComponent :
	public Component
{
	PlayerComponent();
	VP_JSONBODY(PlayerComponent,
		RecoilMode
		,HandName, CameraPosName, CameraName, FirePosName, LongswordName
		, MaxHP, HP, Sencitive, WalkSpeed, RunSpeed, SlideDuration, StaticFriction, DynamicFriction, JumpForce, AirControlPercent, GravityPower, WalkSoundKey1, WalkSoundKey2, Volume_Walk, RunSoundKey1, RunSoundKey2, Volume_Run, JumpSoundkey, Volume_Jump, SlideSoundkey, Volume_Slide, SitSoundKey, Volume_Sit, HurtSoundKey, Volume_Hurt, RecoilProgress, RecoilReturnTime)

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
	VPMath::Vector3 SitCameraPos{};
	float SitHeight{};
	float SitHeightDiff{};
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
	//std::weak_ptr<Entity> SearchedEntity;
	//std::weak_ptr<Entity> PreSearchedEntity;
	//std::weak_ptr<Entity> GunEntity;
	//std::weak_ptr<Entity>ThrowingGunEntity{};


    VisPred::Game::GunType ShootType{};
    bool HasGun=false;
    float GunprogressTime{};
    bool ReadyToShoot{};

    float Volume_Walk   { 0.1f };
    float Volume_Run    { 0.1f };
    float Volume_Jump   { 0.1f };
    float Volume_Slide  { 0.1f };
    float Volume_Sit    { 0.1f };
    float Volume_Hurt   { 0.1f };
    std::string WalkSoundKey1 = "Player_Walk1";
    std::string WalkSoundKey2 = "Player_Walk2";
    std::string RunSoundKey1  = "Player_Run1";
    std::string RunSoundKey2  = "Player_Run2";
    std::string JumpSoundkey  = "Player_Jump";
    std::string SlideSoundkey = "Player_Slide";
    std::string SitSoundKey   = "Player_Sit";
    std::string HurtSoundKey  = "Player_Hurt";
    bool Played_Walk1{};
    bool Played_Jump{};
    bool Played_Slide{};
    bool Played_Sit{};
    bool Played_Run1{};

	bool IsRotated=false;
	bool PlayerCamereShake{};
	bool IsGunRecoiling{};
	bool IsEndReocilReturn{};
	float RecoilProgress{};
	float RecoilReturnTime{};
	VPMath::Quaternion GunRecoilEndQuat{};
	VPMath::Quaternion GunRecoilStartQuat{};
};

