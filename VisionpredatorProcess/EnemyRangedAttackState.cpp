#include "pch.h"
#include "EnemyRangedAttackState.h"
#include "EnemyComponent.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "../PhysxEngine/IPhysx.h"
#include  "Components.h"
#include  "VisPredComponents.h"
void EnemyRangedAttackState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter RangedAttackState");

	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}

#pragma optimize( "", off )
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

	enemyComp->GetComponent<NavAgentComponent>()->IsChase = false;

	// TODO: ���� ��Ÿ�� �����Ҽ��ְ� �ϱ�
	// �����ð� ���� �÷��̾ ����
	if (enemyComp->AttackCycleTimer >= enemyComp->AttackCycleDelay)
	//if (enemyComp->GetComponent<AnimationComponent>()->IsFinished && enemyComp->AttackCycleTimer >= 1.f)
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
				if (enemyComp->MaxShotPerBurst == 0)
				{
					enemyComp->MaxShotPerBurst = 1;
				}
				enemyComp->ShotCount = rand() % enemyComp->MaxShotPerBurst + 1;	// TODO: ���������� �ִ� �� �� ���� �������ִ� ������ �ʿ�.
			}

			auto soundComp = enemyComp->GetComponent<EnemySoundComponent>();
			// ���� ���
			//enemyComp->SceneManager.lock()->SpawnSoundEntity("Pistol",10,false,false , enemyPos);
			enemyComp->SceneManager.lock()->SpawnSoundEntity(soundComp->SoundKey_Attack, soundComp->Volume_Attack, false, false, enemyPos);

			// ����Ʈ ���
			const auto particle = enemyComp->SceneManager.lock()->GetChildEntityComp_HasComp<ParticleComponent>(enemyComp->GetEntityID());
			if (particle != nullptr)
			{
				particle->IsRender = true;
				particle->Restart = true;
			}

			// ���߷� ���
			enemyComp->AttackAccuracy = CalculateAccuracy(*enemyComp);
			const float randomValue = static_cast<float>(rand() % 101);

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
		enemyComp->ShotCount = rand() % enemyComp->MaxShotPerBurst + 1;	// TODO: ���������� �ִ� �� �� ���� �������ִ� ������ �ʿ�.
	}

}
#pragma optimize( "", on )
void EnemyRangedAttackState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit RangedAttackState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}

float EnemyRangedAttackState::CalculateAccuracy(EnemyComponent& enemyComp)
{
	// ���߷��� �����ϴ� ��
	// - ���� �÷��̾ �̵�������, WALk, RUN, JUMP
	// - distance �� ���� ����.
	// 100% ���� ���� ���߷��� �����ϴ� ��ġ�� ���� ���߰ų� �����Ѵ�.

	float accuracy = 100.f; // �⺻ ���߷�

	bool isPlayerMoving = enemyComp.Player->CurrentFSM != VisPred::Game::PlayerFSM::IDLE && enemyComp.Player->CurrentFSM != VisPred::Game::PlayerFSM::CROUCH;

	// �÷��̾ �̵� ���� �� ���߷� ����
	if (isPlayerMoving)
	{
		accuracy -= 20.0f;
	}

	// �Ÿ��� ���߷� ����
	if (enemyComp.DistanceToPlayer <= enemyComp.AccuracyRangeOne)
	{
		// 10m �̳��� ���� �߰� ���� ����
	}
	else if (enemyComp.DistanceToPlayer <= enemyComp.AccuracyRangeTwo)
	{
		accuracy -= 10.0f;
	}
	else if (enemyComp.DistanceToPlayer <= enemyComp.AccuracyRangeThree)
	{
		accuracy -= 20.0f;
	}
	else if (enemyComp.DistanceToPlayer <= enemyComp.AccuracyRangeFour)
	{
		accuracy -= 30.0f;
	}
	else
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
