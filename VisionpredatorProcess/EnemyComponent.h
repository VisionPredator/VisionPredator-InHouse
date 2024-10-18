#pragma once
#include "Component.h"
#include "VisPredStructs.h"

// State
#include "EnemyBehaviorState.h"
#include "EnemyCombatState.h"
#include "EnemyMovementState.h"
#include "EnemyIdleMovementState.h"

#include "PlayerComponent.h"

#include "../PhysxEngine/IPhysx.h"

class SceneManager;
struct EnemyComponent : public Component
{
	EnemyComponent()
	{
		MovementState = &EnemyMovementState::s_Idle;
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

	EnemyBehaviorState* BehaviorState;
	EnemyCombatState* CombatState;
	EnemyMovementState* MovementState;

	PlayerComponent* Player = nullptr;

	std::weak_ptr<SceneManager> SceneManager{};
	Physic::IPhysx* PhysicsManager = nullptr;
};
