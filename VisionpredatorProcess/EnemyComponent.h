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

	VP_JSONBODY(EnemyComponent, HP, CurrentFSM, CurrentAni, EnemyType, HorizontalFOV, VerticalFOV, NearZ, FarZ, 
		IsModelFlipped, NoiseRangeRadius, ChaseRangeRadius, DeadTime, AttackRange, AttackSpeed, AttackCooldown, OnHit, AttackAccuracy, AccuracyRangeOne, AccuracyRangeTwo, AccuracyRangeThree, AccuracyRangeFour)
	float HP{}; 
	VisPred::Game::EnemyStates CurrentFSM= VisPred::Game::EnemyStates::Idle;
	VisPred::Game::EnemyAni CurrentAni = VisPred::Game::EnemyAni::IDLE;
	VisPred::Game::GunType EnemyType= VisPred::Game::GunType::PISTOL;

	// �þ� ����
	float HorizontalFOV = 60.0f;
	float VerticalFOV = 40.0f;
	float NearZ = 0.2f;
	float FarZ = 13.0f;
	bool IsModelFlipped = true;

	// ���� ���� ����
	float NoiseRangeRadius = 5.f;	// 100m
	// �߰� ����
	float ChaseRangeRadius = 40.f;	// 400m

	// ��� �� �Ҹ� �ð�
	float DeadTime = 0.f;	

	/// ���� ������ �ʴ� ����. �����ؾ��Ѵ�.

	float AccuracyRangeOne = 1.f;
	float AccuracyRangeTwo = 3.f;
	float AccuracyRangeThree = 5.f;
	float AccuracyRangeFour = 7.f;

	// ���� �����Ÿ�
	float AttackRange = 10.f;	// �����Ÿ� ���� �÷��̾ ������ ���̻� ���� �ʰ� ���缭 ���.
	// ���� �ӵ�
	float AttackSpeed = 0.f;	// ���� ����/����ü�� ���޾� �߻��ϴ� ���� ������ �ð�
	// ���� ��ٿ� �ð�
	float AttackCooldown = 0.f;		// ����Ŭ�� ������ ���ݰ� ���� ����Ŭ�� ù ��° ���� ������ �ð�

	float currentAttackTime = 0.f;  // ������ �������� Ȯ���ϱ� ���� Ÿ�̸�
	bool IsCoolingDown = false;		// ��ٿ� ���¸� ����

	float AttackAccuracy = 100.f;	// ���� ���߷�

	bool OnHit = false;	// �ǰ� �޴� ������


	float DistanceToPlayer = -1.f;	// �÷��̾� ������ �Ÿ�

	// Chase ���� �� ���������� ���� �� �÷��̾��� ��ġ
	VPMath::Vector3 LastDetectedPlayerPos;

	EnemyBehaviorState* BehaviorState;
	EnemyCombatState* CombatState;
	EnemyMovementState* MovementState;

	PlayerComponent* Player = nullptr;

	std::weak_ptr<SceneManager> SceneManager{};
	Physic::IPhysx* PhysicsManager = nullptr;
	Graphics::Interface* Graphics = nullptr;
};
