#pragma once
#include "Component.h"
#include "VisPredStructs.h"

// State
#include "StatesInclude.h"

#include "PlayerComponent.h"

#include "../PhysxEngine/IPhysx.h"
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

	VP_JSONBODY(EnemyComponent, HP, CurrentFSM, CurrentAni, EnemyType, HorizontalFOV, VerticalFOV, NearZ, FarZ, IsModelFlipped)
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

	// 공격 사정거리
	float AttackRange = 10.f;	// 사정거리 내에 플레이어가 있으면 더이상 쫓지 않고 멈춰서 사격.

	bool OnHit = false;	// 피격 받는 중인지

	// Chase 상태 중 마지막으로 포착 된 플레이어의 위치
	VPMath::Vector3 LastDetectedPlayerPos;

	EnemyBehaviorState* BehaviorState;
	EnemyCombatState* CombatState;
	EnemyMovementState* MovementState;

	PlayerComponent* Player = nullptr;

	std::weak_ptr<SceneManager> SceneManager{};
	Physic::IPhysx* PhysicsManager = nullptr;
	Graphics::Interface* Graphics = nullptr;
};
