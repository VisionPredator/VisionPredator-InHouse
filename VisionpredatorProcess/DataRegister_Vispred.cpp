#include "pch.h"
#include "DataRegister_Vispred.h"

#include "VisPredComponents.h"
#include "VisPredStructs.h"
void VispredRegister::Register_Metadata()
{
	VispredRegister::Register_Components();
	VispredRegister::Register_EnumClass();
	VispredRegister::Register_Structs();
	VispredRegister::Register_Value();
	VispredRegister::Register_VPMath();
}

void VispredRegister::Register_Components()
{
	META_ADD_COMP(BulletComponent, BulletComponent::SparkParticleName ,BulletComponent::Damage, BulletComponent::Speed);
	META_ADD_COMP(PlayerComponent
		, PlayerComponent::IsAttacking
		, PlayerComponent::CurrentFSM
		, PlayerComponent::CameraPosName, PlayerComponent::CameraName, PlayerComponent::FirePosName, PlayerComponent::HandName, PlayerComponent::VPHandName, PlayerComponent::LongswordName
		, PlayerComponent::NonDamageTime
		, PlayerComponent::MaxHP, PlayerComponent::HP
		, PlayerComponent::Sencitive
		, PlayerComponent::SearchDistance
		, PlayerComponent::JumpForce
		, PlayerComponent::WalkSpeed, PlayerComponent::RunSpeed
		, PlayerComponent::SlideDuration
		, PlayerComponent::DashDuration
		, PlayerComponent::SlideMultiple
		, PlayerComponent::DashMultiple
		, PlayerComponent::AirControlPercent
		, PlayerComponent::GravityPower
		, PlayerComponent::StaticFriction
		, PlayerComponent::VPGageCoolTime
		, PlayerComponent::TransformationTime
	);
	META_ADD_COMP(InterectiveComponent, InterectiveComponent::IsInterective, InterectiveComponent::SearcehdColor, InterectiveComponent::Soundkey, InterectiveComponent::Volume);
	META_ADD_COMP(PlayerSoundComponent
		, PlayerSoundComponent::Volume_Walk
		, PlayerSoundComponent::Volume_Run
		, PlayerSoundComponent::Volume_Jump
		, PlayerSoundComponent::Volume_Landing
		, PlayerSoundComponent::Volume_Slide
		, PlayerSoundComponent::Volume_Dash
		, PlayerSoundComponent::Volume_Sit
		, PlayerSoundComponent::Volume_Hurt
		, PlayerSoundComponent::Volume_Death
		, PlayerSoundComponent::Volume_GunDraw
		, PlayerSoundComponent::Volume_GunThrow
		, PlayerSoundComponent::Volume_Sword1
		, PlayerSoundComponent::Volume_Sword2
		, PlayerSoundComponent::Volume_Sword3
		, PlayerSoundComponent::Volume_VPAttack1
		, PlayerSoundComponent::Volume_VPAttack2
		, PlayerSoundComponent::Volume_Heal
		, PlayerSoundComponent::Volume_Transformation
		, PlayerSoundComponent::SoundKey_Walk1
		, PlayerSoundComponent::SoundKey_Walk2
		, PlayerSoundComponent::SoundKey_Run1
		, PlayerSoundComponent::SoundKey_Run2
		, PlayerSoundComponent::SoundKey_Jump
		, PlayerSoundComponent::Volume_Landing
		, PlayerSoundComponent::SoundKey_Slide
		, PlayerSoundComponent::SoundKey_Dash
		, PlayerSoundComponent::SoundKey_Sit
		, PlayerSoundComponent::SoundKey_Hurt
		, PlayerSoundComponent::SoundKey_Death
		, PlayerSoundComponent::SoundKey_GunDraw
		, PlayerSoundComponent::SoundKey_GunThrow
		, PlayerSoundComponent::SoundKey_Sword1
		, PlayerSoundComponent::SoundKey_Sword2
		, PlayerSoundComponent::SoundKey_Sword3
		, PlayerSoundComponent::SoundKey_VPAttack1
		, PlayerSoundComponent::SoundKey_VPAttack2
		, PlayerSoundComponent::SoundKey_Heal
		, PlayerSoundComponent::SoundKey_Transformation);
	META_ADD_COMP(TrunComponent, TrunComponent::MoveTime, TrunComponent::Is_X, TrunComponent::Angle, TrunComponent::Finished);
	META_ADD_COMP(AreaAttackComponent, AreaAttackComponent::IdentityAttach, AreaAttackComponent::Damage);
	META_ADD_COMP(PlayerMeleeComponent, PlayerMeleeComponent::DefalutPrefab, PlayerMeleeComponent::DashPrefab, PlayerMeleeComponent::AttackMode, PlayerMeleeComponent::SwordLength, PlayerMeleeComponent::SwordAngle, PlayerMeleeComponent::SwordDamage, PlayerMeleeComponent::VPLength, PlayerMeleeComponent::VPAngle, PlayerMeleeComponent::VPDamage);

	META_ADD_COMP(EnemyComponent
		, EnemyComponent::HP
		, EnemyComponent::CurrentFSM
		, EnemyComponent::CurrentAni
		, EnemyComponent::EnemyType
		, EnemyComponent::HorizontalFOV, EnemyComponent::VerticalFOV
		, EnemyComponent::NearZ, EnemyComponent::FarZ
		, EnemyComponent::IsModelFlipped
		, EnemyComponent::NoiseRangeRadius, EnemyComponent::ChaseRangeRadius
		, EnemyComponent::DeadTime
		, EnemyComponent::AttackRange, EnemyComponent::AttackSpeed, EnemyComponent::AttackCooldown
		, EnemyComponent::OnHit
		, EnemyComponent::AttackAccuracy
		, EnemyComponent::AccuracyRangeOne, EnemyComponent::AccuracyRangeTwo, EnemyComponent::AccuracyRangeThree, EnemyComponent::AccuracyRangeFour
		, EnemyComponent::AttackPower
	);
	META_ADD_COMP(EnemySoundComponent
		, EnemySoundComponent::Volume_Run
		, EnemySoundComponent::Volume_Hurt
		, EnemySoundComponent::Volume_Attack
		, EnemySoundComponent::Volume_Death1
		, EnemySoundComponent::Volume_Death2
		, EnemySoundComponent::Volume_Death3
		, EnemySoundComponent::SoundKey_Run
		, EnemySoundComponent::SoundKey_Hurt
		, EnemySoundComponent::SoundKey_Attack
		, EnemySoundComponent::SoundKey_Death1
		, EnemySoundComponent::SoundKey_Death2
		, EnemySoundComponent::SoundKey_Death3
		)

	META_ADD_COMP(GunComponent, GunComponent::Type, GunComponent::BulletPrefab, GunComponent::MuzzleEffectPointLightPrefab,GunComponent::BulletSpeed, GunComponent::ThrowDamage, GunComponent::Damage1, GunComponent::Damage2, GunComponent::Damage3, GunComponent::BulletSize, GunComponent::ShotGunDistance, GunComponent::SoundKey_GunSound,GunComponent::SoundKey_GunDrop, GunComponent::Volume_GunSound,GunComponent::Volume_GunDrop, GunComponent::CoolTime, GunComponent::CurrentBullet, GunComponent::Bullets, GunComponent::RecoilPos, GunComponent::RecoilMaxXY, GunComponent::RecoilBack,GunComponent::RecoilTime, GunComponent::RecoilPercent);
	META_ADD_COMP(AimUIComponent
		, AimUIComponent::Killed
		, AimUIComponent::Aimed
		, AimUIComponent::Attacked
		, AimUIComponent::Interected
	);
	META_ADD_COMP(ShotGunBulletComponent, ShotGunBulletComponent::SparkParticleName,ShotGunBulletComponent::Damage1, ShotGunBulletComponent::Damage2, ShotGunBulletComponent::Damage3, ShotGunBulletComponent::Distance, ShotGunBulletComponent::Speed);
	META_ADD_COMP(VPUIComponent
		, VPUIComponent::ChangeColor
		, VPUIComponent::FullImage
		, VPUIComponent::GageImage
	)

	META_ADD_COMP(DoorComponent, DoorComponent::SoundKey, DoorComponent::Volume, DoorComponent::LeftDoor, DoorComponent::RightDoor, DoorComponent::OpenTime, DoorComponent::IsUseserble, DoorComponent::MoveDistance);
	META_ADD_COMP(SectorClearComponent, SectorClearComponent::OpenDoorIdentity, SectorClearComponent::PreDoorIdentity);
	META_ADD_COMP(DoorOpenerComponent, DoorOpenerComponent::Dummy);
	META_ADD_COMP(WeaponBoxComponent, WeaponBoxComponent::SpawnPrefabs, WeaponBoxComponent::TopMesh, WeaponBoxComponent::OpenAngle, WeaponBoxComponent::OpenTime, WeaponBoxComponent::SpawnOffset, WeaponBoxComponent::SpawnDirection, WeaponBoxComponent::SpawnSpeed);
	META_ADD_COMP(CabinetComponent
		, CabinetComponent::RightDoor, CabinetComponent::LeftDoor, CabinetComponent::OpenAngle, CabinetComponent::OpenTime
	
	
	
	
	);
	META_ADD_COMP(EntityRemoverComponet, EntityRemoverComponet::temp);
	META_ADD_COMP(IdentityRemoverComponent, IdentityRemoverComponent::RemoveIdentitys);
	META_ADD_COMP(HPReducerComponent, HPReducerComponent::MaxHP, HPReducerComponent::DownHP);
	META_ADD_COMP(AutoPickComponent, AutoPickComponent::IsAuto, AutoPickComponent::PickUps);
	META_ADD_COMP(VPDetectionComponent, VPDetectionComponent::Length, VPDetectionComponent::EnemyColor, VPDetectionComponent::InterectColor);
	META_ADD_COMP(DoOnceComponent, DoOnceComponent::temp);
	META_ADD_COMP(DoOnceResetComponent, DoOnceResetComponent::ResetIdentitys);
	META_ADD_COMP(SpawnerComponent, SpawnerComponent::SpawnPrefab, SpawnerComponent::SpawnTransform);
	META_ADD_COMP(DoorAccessComponent, DoorAccessComponent::Open, DoorAccessComponent::DoorIdentitys);
	META_ADD_COMP(SpawnChildComponent
		, SpawnChildComponent::Prefab1
		, SpawnChildComponent::Prefab1_Pose
		, SpawnChildComponent::Prefab2
		, SpawnChildComponent::Prefab2_Pose
		, SpawnChildComponent::Prefab3
		, SpawnChildComponent::Prefab3_Pose
		, SpawnChildComponent::Prefab4
		, SpawnChildComponent::Prefab4_Pose
		, SpawnChildComponent::Prefab5
		, SpawnChildComponent::Prefab5_Pose
	);
	META_ADD_COMP(QuestComponent, QuestComponent::PlayerIdentity, QuestComponent::QuestType,  QuestComponent::IsStarted, QuestComponent::IsCleared);
	META_ADD_COMP(MainQuestComponent, MainQuestComponent::QuestSequence
		,MainQuestComponent::Volume_Subquest
		,MainQuestComponent::Volume_Mainquest
		, MainQuestComponent::SounKey_Subquest
		,MainQuestComponent::SounKey_Mainquest
		
	);
	META_ADD_COMP(PlayerUIComponent, PlayerUIComponent::AimUI, PlayerUIComponent::FadeUI, PlayerUIComponent::HitUI, PlayerUIComponent::HPGage, PlayerUIComponent::InterectionUI, PlayerUIComponent::Player, PlayerUIComponent::VPeyeUI, PlayerUIComponent::WeaponUI);
	META_ADD_COMP(		CursorComponent, CursorComponent::ShowCursor, CursorComponent::CursorImage, CursorComponent::CursorScale	);
	META_ADD_COMP(HitUIComponent, HitUIComponent::IsHitUIOn, HitUIComponent::ProgressTime, HitUIComponent::DurationTime);
}

void VispredRegister::Register_EnumClass()
{
	using namespace VisPred::Game;
	META_ADD_ENUMCLASS(PlayerFSM
		,PlayerFSM::IDLE
		,PlayerFSM::WALK
		,PlayerFSM::RUN
		,PlayerFSM::CROUCH
		,PlayerFSM::Dash_Slide
		,PlayerFSM::JUMP
		,PlayerFSM::DIE
		,PlayerFSM::DIE_END
		,PlayerFSM::Transformation);
	META_ADD_ENUMCLASS(PlayerMelee, PlayerMelee::Sword_First, PlayerMelee::Sword_Second, PlayerMelee::Sword_Third, PlayerMelee::Sword_Fourth, PlayerMelee::VP_Left, PlayerMelee::VP_Right, PlayerMelee::END);
	META_ADD_ENUMCLASS(GunRecoilMode, GunRecoilMode::ReturnToEndAim, GunRecoilMode::ReturnToMiddle);
	META_ADD_ENUMCLASS(EnemyStates, EnemyStates::Idle, EnemyStates::Chase, EnemyStates::Patrol, EnemyStates::Dead);
	META_ADD_ENUMCLASS(EnemyAni, EnemyAni::ATTACK, EnemyAni::IDLE, EnemyAni::CHASE, EnemyAni::JUMP, EnemyAni::WALK, EnemyAni::DIE, EnemyAni::ATTACKED, EnemyAni::BACKWALK, EnemyAni::ATTACK_IDLE);
	META_ADD_ENUMCLASS(GunType, GunType::NONE, GunType::PISTOL, GunType::RIFLE, GunType::SHOTGUN, GunType::END);
	META_ADD_ENUMCLASS(PlayerAni
		, PlayerAni::ToAttack_Pistol
		, PlayerAni::ToAttack_Rifle
		, PlayerAni::ToAttack_ShotGun
		, PlayerAni::ToIdle01_Pistol
		, PlayerAni::ToIdle01_Rifle
		, PlayerAni::ToIdle01_ShotGun
		, PlayerAni::ToIdle02_Pistol
		, PlayerAni::ToIdle02_Rifle
		, PlayerAni::ToIdle02_ShotGun
		, PlayerAni::Tohook_Sword
		, PlayerAni::Tohook_Pistol
		, PlayerAni::Tohook_Rifle
		, PlayerAni::Tohook_ShotGun
		, PlayerAni::Tointeraction
		, PlayerAni::ToAttack1_Sword
		, PlayerAni::ToAttack2_Sword
		, PlayerAni::ToAttack3_Sword
		, PlayerAni::ToIdle01_Sword
		, PlayerAni::ToIdle02_Sword
		, PlayerAni::ToThrow_Pistol
		, PlayerAni::ToThrow_Rifle
		, PlayerAni::ToThrow_ShotGun
	);
	META_ADD_ENUMCLASS(QuestType
		, QuestType::VPMOVE
		, QuestType::VPJUMP
		, QuestType::VPDASH
		, QuestType::VPCHANGE
		, QuestType::PLAYERSHOOT
		, QuestType::PLAYERRUN
		, QuestType::PLAYERJUMP
		, QuestType::PLAYERCROUCH
		, QuestType::PLAYERSLIDE
		, QuestType::PLAYERATTACK
		, QuestType::PLAYERPICKUP
		, QuestType::PLAYERTHROW
		, QuestType::PLAYERINTERECT);







	META_ADD_ENUMCLASS(VPAni
		, VPAni::ToVP_attack_L
		, VPAni::ToVP_attack_R
		, VPAni::ToVP_Idle
		, VPAni::ToVP_dash
		, VPAni::ToVP_jump
		, VPAni::ToVP_run
		, VPAni::ToVP_draw
	);
}

void VispredRegister::Register_Structs()
{

}

void VispredRegister::Register_Value()
{
}

void VispredRegister::Register_VPMath()
{
}
