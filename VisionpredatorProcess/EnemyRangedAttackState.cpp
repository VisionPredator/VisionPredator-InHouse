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

void EnemyRangedAttackState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	DetectTarget(*enemyComp, deltaTime);	/// TEMP!!!!!!!
	// 일정시간 마다 플레이어를 공격
// 명중률에 따라 피격 정도를 다르게한다.
	auto transform = enemyComp->GetComponent<TransformComponent>();
	auto enemyPos = transform->World_Location;
	auto rotation = transform->World_Rotation;

	enemyComp->currentAttackTime += deltaTime;

	const auto playerTransform = enemyComp->Player->GetComponent<TransformComponent>();
	const VisPred::SimpleMath::Vector3 playerPos = VisPred::SimpleMath::Vector3{
		playerTransform->WorldTransform._41, playerTransform->WorldTransform._42, playerTransform->WorldTransform._43
	};
	auto targetDir = playerPos - enemyPos;
	targetDir.Normalize();

	// 명중률에 관여하는 것
	// - 현재 플레이어가 이동중인지, WALk, RUN, JUMP
	// - distance 에 따라 감소.
	// 100% 에서 여러 명중률에 관여하는 수치에 따라 낮추거나 유지한다.

	//ChangeCurrentAnimation(enemyComp, VisPred::Game::EnemyAni::ATTACK, 1.f, 0.1f, false);

	enemyComp->GetComponent<NavAgentComponent>()->IsChase = false;

	if ( enemyComp->currentAttackTime >= 1.f)
	//if (enemyComp->GetComponent<AnimationComponent>()->IsFinished && enemyComp->currentAttackTime >= 1.f)
	{
		//enemyComp->SceneManager.lock()->SpawnEditablePrefab("../Data/Prefab/Pistol_Bullet.prefab", enemyPos, rotation);
		//enemyComp->SceneManager.lock()->SpawnEditablePrefab("../Data/Prefab/Sound_Pistol.prefab", enemyPos, rotation);
		enemyComp->SceneManager.lock()->SpawnSoundEntity("Sound_Pistol",10,false,false , enemyPos);

		enemyComp->currentAttackTime = 0.f;
		   
		//ChangeCurrentState(enemyComp, &EnemyCombatState::s_Idle);
		//ChangeCurrentAnimation(*enemyComp, VisPred::Game::EnemyAni::CHASE, 0.005f, 0.005, false);	// Attack Idle 이 있었으면 넣었는데 없어서 이걸로 대체. 근데 어색해서 Attack 애니메이션은 안넣기..


		enemyComp->AttackAccuracy = CalculateAccuracy(*enemyComp);
		// 난수 생성 (0 ~ 100 범위)
		float randomValue = static_cast<float>(rand() % 101);

		// 명중률에 따라 사격
		if (randomValue <= enemyComp->AttackAccuracy)
		{
			const uint32_t detectedObjID = enemyComp->PhysicsManager->RaycastToHitActorFromLocation_Ignore(enemyComp->GetEntityID(), enemyPos, targetDir, enemyComp->FarZ).EntityID;
			if (detectedObjID == enemyComp->Player->GetEntityID())
			{
				enemyComp->Player->HP -= enemyComp->AttackPower;
				Log::GetClientLogger()->warn("Attack Succeed!!!");
			}
			else
			{
				Log::GetClientLogger()->warn("Missed Target !!!!!!!!!!!");
			}
		}
		// ChangeCurrentState(enemyComp, &EnemyMovementState::s_Attack);
		ChangeCurrentState(enemyComp, &EnemyCombatState::s_Idle);
		return;
	}
	/*else
	{
		if (enemyComp->GetComponent<NavAgentComponent>()->IsChase == false)
			ChangeCurrentState(enemyComp, &EnemyMovementState::s_IdleAttack);
	}*/


	//if (enemyComp->GetComponent<NavAgentComponent>()->IsChase)
	//	
}

void EnemyRangedAttackState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit RangedAttackState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	//ChangeCurrentState(enemyComp, &EnemyMovementState::s_IdleAttack);
}

float EnemyRangedAttackState::CalculateAccuracy(EnemyComponent& enemyComp)
{
	float accuracy = 100.f; // 기본 명중률

	bool isPlayerMoving = enemyComp.Player->CurrentFSM != VisPred::Game::PlayerFSM::IDLE && enemyComp.Player->CurrentFSM != VisPred::Game::PlayerFSM::CROUCH;

	// 플레이어가 이동 중일 때 명중률 감소
	if (isPlayerMoving)
	{
		accuracy -= 10.0f;
	}

	// 거리별 명중률 감소
	if (enemyComp.DistanceToPlayer <= enemyComp.AccuracyRangeOne)
	{
		// 10m 이내일 때는 추가 감소 없음
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

	// 명중률이 0 이하로 내려가지 않도록 보장
	if (accuracy < 0.0f)
	{
		accuracy = 0.0f;
	}

	return accuracy;
}
