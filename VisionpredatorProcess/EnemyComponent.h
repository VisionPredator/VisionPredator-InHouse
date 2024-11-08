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

	// ���߷��� �����ϴ� �Ÿ�������
	float AccuracyRangeOne = 1.f;
	float AccuracyRangeTwo = 3.f;
	float AccuracyRangeThree = 5.f;
	float AccuracyRangeFour = 7.f;
	// �Ÿ��� ���ҵǴ� ���߷�
	float AccuracyPenaltyOne = 20.f;;
	float AccuracyPenaltyTwo = 30.0f;
	float AccuracyPenaltyThree = 40.0f;
	float AccuracyPenaltyFour = 50.0f;

	// ���ݷ�
	uint32_t AttackPower = 1.f;

	float AttackRange = 10.f;	// �����Ÿ� ���� �÷��̾ ������ ���̻� ���� �ʰ� ���缭 ���.
	bool OnHit = false;	// �ǰ� �޴� ������

	// Chase ���� �� ���������� ������ �÷��̾��� ��ġ
	VPMath::Vector3 LastDetectedPlayerPos;

	EnemyBehaviorState* BehaviorState;
	EnemyCombatState* CombatState;
	EnemyMovementState* MovementState;

	PlayerComponent* Player = nullptr;

	std::weak_ptr<SceneManager> SceneManager{};
	Physic::IPhysx* PhysicsManager = nullptr;
	Graphics::Interface* Graphics = nullptr;
	Sound::ISound* SoundEngine = nullptr;

	/// ���� ������ �ʴ� ����. �����ؾ��Ѵ�.
	// ���� �ӵ�
	// ���� ��ٿ� �ð�
	bool IsCoolingDown = false;		// ��ٿ� ���¸� ����

	// ���� ����
	float AttackCycleDelay = 1.f;			// ����Ŭ�� ������ ���ݰ� ���� ����Ŭ�� ù ��° ���� ������ �ð�
	float AttackCycleTimer = 0.f;			// ���� ����Ŭ�� �󸶳� ����Ǿ������� ��Ÿ���� Ÿ�̸�

	float ConsecutiveAttackDelay = 0.3f;		// ���� ����/����ü�� ���޾� �߻��ϴ� ���� ������ �ð�
	float ConsecutiveAttackTimer = 0.f;		// ������ �����Ѱ� �󸶳� �Ǿ�����.

	int MaxShotPerBurst = 0;	// ���� ���� ������ �߻��� �ִ� �߻� ��

	float DistanceToPlayer = -1.f;		// �÷��̾� ������ �Ÿ�
	float AttackAccuracy = 100.f;		// ���� ���߷�

	bool HeatComplete = true;
	int ShotCount = 3;
};
