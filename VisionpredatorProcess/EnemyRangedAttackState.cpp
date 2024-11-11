#include "pch.h"
#include "EnemyRangedAttackState.h"
#include "EnemyComponent.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "../PhysxEngine/IPhysx.h"
#include  "Components.h"
#include  "VisPredComponents.h"
#include <random>
void EnemyRangedAttackState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter RangedAttackState");

	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}

void EnemyRangedAttackState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	const auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	DetectTarget(*enemyComp, deltaTime);	/// TEMP!!!!!!!
	const auto transform = enemyComp->GetComponent<TransformComponent>();
	const auto enemyPos = transform->World_Location;
	auto rotation = transform->World_Rotation;

	enemyComp->AttackCycleTimer += deltaTime;
	enemyComp->ConsecutiveAttackTimer += deltaTime;

	const auto playerTransform = enemyComp->Player->GetComponent<TransformComponent>();
	const auto playerPos = VisPred::SimpleMath::Vector3{
		playerTransform->WorldTransform._41, playerTransform->WorldTransform._42, playerTransform->WorldTransform._43
	};
	auto targetDir = playerPos - enemyPos;
	targetDir.Normalize();

	// ���� ������ �ʱ�ȭ
	std::random_device rd;  // �ϵ���� ��Ʈ���Ǹ� ������� �õ� ����
	std::mt19937 gen(rd()); // Mersenne Twister ���� ������

	// �����ð� ���� �÷��̾ ����
	if (enemyComp->AttackCycleTimer >= enemyComp->AttackCycleDelay)
	{
		// ���� �߻� Ÿ�̸Ӱ� �����ǰ� ���� ���� ���� Ƚ���� ���� ���� �߻�.
		if (enemyComp->ConsecutiveAttackTimer >= enemyComp->ConsecutiveAttackDelay && enemyComp->ShotCount > 0)
		{
			enemyComp->ConsecutiveAttackTimer = 0;
			enemyComp->ShotCount--;

			if (enemyComp->ShotCount <= 0)
			{
				// Ÿ�̸� �ʱ�ȭ
				enemyComp->AttackCycleTimer = 0.f;
				if (enemyComp->MaxShotPerBurst <= 0)	// ����ó��
				{
					enemyComp->MaxShotPerBurst = 1;
				}
				std::uniform_int_distribution dist(1, enemyComp->MaxShotPerBurst);
				enemyComp->ShotCount = dist(gen);
			}

			const auto soundComp = enemyComp->GetComponent<EnemySoundComponent>();

			// ���� ���� ���
			enemyComp->SceneManager.lock()->SpawnSoundEntity(
				soundComp->SoundKey_Attack, 
				soundComp->Volume_Attack, 
				false, 
				false, 
				enemyPos
			);

			// ���� ����Ʈ ���
			const auto particle = enemyComp->SceneManager.lock()->GetChildEntityComp_HasComp<ParticleComponent>(enemyComp->GetEntityID());
			if (particle != nullptr)
			{
				particle->IsRender = true;
				particle->Restart = true;
			}

			// ���߷� ���
			enemyComp->AttackAccuracy = CalculateAccuracy(*enemyComp);

			std::random_device rd;  // �ϵ���� ��Ʈ���Ǹ� ������� �õ� ����
			std::mt19937 gen(rd()); // Mersenne Twister ���� ������
			std::uniform_int_distribution dist(0, 100);
			const float randomValue = static_cast<float>(dist(gen));

			// ���߷��� ���� ���
			if (randomValue <= enemyComp->AttackAccuracy)
			{
				const uint32_t detectedObjID = enemyComp->PhysicsManager->RaycastActorAtPose_Ignore(enemyComp->GetEntityID(), enemyPos, targetDir, enemyComp->FarZ).EntityID;
				if (detectedObjID == enemyComp->Player->GetEntityID())
				{
					EventManager::GetInstance().ImmediateEvent("OnDamaged", std::make_pair<uint32_t, int >(enemyComp->Player->GetEntityID(), enemyComp->AttackPower));
				}
			}
			ChangeCurrentState(enemyComp, &EnemyCombatState::s_Idle);
		}
	}

	if (enemyComp->ShotCount <= 0)
	{
		// Ÿ�̸� �ʱ�ȭ
		enemyComp->AttackCycleTimer = 0.f;
		if (enemyComp->MaxShotPerBurst <= 0)	// ����ó��
		{
			enemyComp->MaxShotPerBurst = 1;
		}
		std::uniform_int_distribution dist(1, enemyComp->MaxShotPerBurst);
		enemyComp->ShotCount = dist(gen);
	}

}

void EnemyRangedAttackState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit RangedAttackState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}

float EnemyRangedAttackState::CalculateAccuracy(const EnemyComponent& enemyComp)
{
	// ���߷��� �����ϴ� ��
	// - ���� �÷��̾ �̵�������, WALk, RUN, JUMP
	// - distance �� ���� ����.
	// 100% ���� ���� ���߷��� �����ϴ� ��ġ�� ���� ���߰ų� �����Ѵ�.

	float accuracy = 100.f; // �⺻ ���߷�

	const bool isPlayerWalking = enemyComp.Player->CurrentFSM == VisPred::Game::PlayerFSM::WALK;
	const bool isPlayerRunning = enemyComp.Player->CurrentFSM == VisPred::Game::PlayerFSM::RUN;
	const bool isPlayerJumping = enemyComp.Player->CurrentFSM == VisPred::Game::PlayerFSM::JUMP;

	// �Ÿ��� ���߷� ����
	if (enemyComp.DistanceToPlayer <= enemyComp.AccuracyRangeOne)
	{
		// 10m �̳��� ���� �߰� ���� ����
	}
	else if (enemyComp.DistanceToPlayer <= enemyComp.AccuracyRangeTwo)
	{
		accuracy -= enemyComp.AccuracyPenaltyOne;
	}
	else if (enemyComp.DistanceToPlayer <= enemyComp.AccuracyRangeThree)
	{
		accuracy -= enemyComp.AccuracyPenaltyTwo;
	}
	else if (enemyComp.DistanceToPlayer <= enemyComp.AccuracyRangeFour)
	{
		accuracy -= enemyComp.AccuracyPenaltyThree;
	}
	else
	{
		accuracy -= enemyComp.AccuracyPenaltyFour;
	}

	// �÷��̾ �̵� ���� �� ���߷� ����
	if (isPlayerWalking || isPlayerJumping)
	{
		accuracy -= 20.0f;
	}
	else if (isPlayerRunning)
	{
		accuracy -= 40.0f;
	}

	// ���߷��� 0 ���Ϸ� �������� �ʵ��� ����
	if (accuracy < 0.0f)
	{
		accuracy = 0.0f;
	}

	return accuracy;
}
