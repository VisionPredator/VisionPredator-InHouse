#pragma once
#include "Component.h"
#include "VisPredStructs.h"

// State
#include "StatesInclude.h"

#include "PlayerComponent.h"

#include "../PhysxEngine/IPhysx.h"
#include "../SoundEngine/ISound.h"
#include "../VPGraphics/IGraphics.h"

class SceneManager;
struct EnemyComponent : public Component
{
	EnemyComponent()
	{
		MovementState = &EnemyMovementState::s_Idle;
		BehaviorState = &EnemyBehaviorState::s_Idle;
		CombatState = &EnemyCombatState::s_Idle;
	}

	VP_JSONBODY(EnemyComponent, 
		HP, 
		CurrentFSM, 
		CurrentAni, 
		EnemyType, 
		HorizontalFOV, VerticalFOV, 
		NearZ, FarZ, 
		IsModelFlipped, 
		NoiseRangeRadius, 
		ChaseRangeRadius, 
		DeadTime, 
		AttackRange, 
		ConsecutiveAttackDelay, 
		AttackCycleDelay, 
		OnHit, 
		AttackAccuracy, 
		AccuracyRangeOne, AccuracyRangeTwo, AccuracyRangeThree, AccuracyRangeFour,
		AccuracyPenaltyOne, AccuracyPenaltyTwo, AccuracyPenaltyThree, AccuracyPenaltyFour,
		AttackPower,
		AttackCycleDelay,
		ConsecutiveAttackDelay,
		MaxShotPerBurst
		)
	float HP{}; 
	VisPred::Game::EnemyStates CurrentFSM= VisPred::Game::EnemyStates::Idle;
	VisPred::Game::EnemyAni CurrentAni = VisPred::Game::EnemyAni::IDLE;
	VisPred::Game::GunType EnemyType= VisPred::Game::GunType::PISTOL;

	// 시야 범위
	float HorizontalFOV = 60.0f;
	float VerticalFOV = 40.0f;
	float NearZ = 0.2f;
	float FarZ = 13.0f;
	bool IsModelFlipped = true;

	// 소음 감지 범위
	float NoiseRangeRadius = 5.f;	// 100m
	// 추격 범위
	float ChaseRangeRadius = 40.f;	// 400m

	// 사망 후 소멸 시간
	float DeadTime = 0.f;	

	// 명중률에 관여하는 거리범위들
	float AccuracyRangeOne = 1.f;
	float AccuracyRangeTwo = 3.f;
	float AccuracyRangeThree = 5.f;
	float AccuracyRangeFour = 7.f;
	// 거리별 감소되는 명중률
	float AccuracyPenaltyOne = 20.f;;
	float AccuracyPenaltyTwo = 30.0f;
	float AccuracyPenaltyThree = 40.0f;
	float AccuracyPenaltyFour = 50.0f;

	// 공격력
	uint32_t AttackPower = 1.f;

	float AttackRange = 10.f;	// 사정거리 내에 플레이어가 있으면 더이상 쫓지 않고 멈춰서 사격.
	bool OnHit = false;	// 피격 받는 중인지

	// Chase 상태 중 마지막으로 포착된 플레이어의 위치
	VPMath::Vector3 LastDetectedPlayerPos;

	EnemyBehaviorState* BehaviorState;
	EnemyCombatState* CombatState;
	EnemyMovementState* MovementState;

	PlayerComponent* Player = nullptr;

	std::weak_ptr<SceneManager> SceneManager{};
	Physic::IPhysx* PhysicsManager = nullptr;
	Graphics::Interface* Graphics = nullptr;
	Sound::ISound* SoundEngine = nullptr;

	/// 아직 쓰이지 않는 변수. 연결해야한다.
	// 공격 속도
	// 공격 쿨다운 시간
	bool IsCoolingDown = false;		// 쿨다운 상태를 관리

	// 추후 수정
	float AttackCycleDelay = 1.f;			// 사이클의 마지막 공격과 다음 사이클의 첫 번째 공격 사이의 시간
	float AttackCycleTimer = 0.f;			// 현재 사이클이 얼마나 진행되었는지를 나타내는 타이머

	float ConsecutiveAttackDelay = 0.3f;		// 여러 공격/투사체를 연달아 발사하는 공격 사이의 시간
	float ConsecutiveAttackTimer = 0.f;		// 공격을 시작한게 얼마나 되었는지.

	int MaxShotPerBurst = 0;	// 연속 공격 내에서 발사할 최대 발사 수

	float DistanceToPlayer = -1.f;		// 플레이어 까지의 거리
	float AttackAccuracy = 100.f;		// 공격 명중률

	bool HeatComplete = true;
	int ShotCount = 3;
};
