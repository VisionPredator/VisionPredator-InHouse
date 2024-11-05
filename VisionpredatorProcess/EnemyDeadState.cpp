#include "pch.h"
#include "EnemyDeadState.h"
#include "EnemyComponent.h"
#include "RigidBodyComponent.h"
#include "StatesInclude.h"
#include "EventManager.h"
#include "../VPEngine/SceneManager.h"

void EnemyDeadState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter DeadState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	if (enemyComp->HasComponent<ControllerComponent>())
	{
		enemyComp->PhysicsManager->RemoveController(enemyComp->GetEntityID());
	}
	EventManager::GetInstance().ImmediateEvent("OnRemoveNavAgent", enemyComp->GetEntityID());
	auto pose = enemyComp->GetComponent<TransformComponent>()->World_Location;
	pose.y += 0.4;
	int temp = VPMath::Random_int(0, 2);
	switch (temp)
	{
	case 0:
		enemyComp->SceneManager.lock()->SpawnEditablePrefab("../Data/Prefab/Pistol.prefab", pose, VPMath::Vector3{}, { .2,.2,.2 });
		break;
	case 1:
		enemyComp->SceneManager.lock()->SpawnEditablePrefab("../Data/Prefab/ShotGun.prefab", pose, VPMath::Vector3{}, { .2,.2,.2 });
		break;
	case 2:
		enemyComp->SceneManager.lock()->SpawnEditablePrefab("../Data/Prefab/Rifle.prefab", pose, VPMath::Vector3{}, { .2,.2,.2 });
		break;
	default:
		break;
	}
	ChangeCurrentState(enemyComp, &EnemyCombatState::s_Idle);
	ChangeCurrentState(enemyComp, &EnemyMovementState::s_Idle);
	enemyComp->MovementState->Enter(enemyComp);
	EventManager::GetInstance().ImmediateEvent("OnUpdateSector", enemyComp->GetEntityID());
}

void EnemyDeadState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	constexpr float timeToDelete = 20.f;

	enemyComp->DeadTime += deltaTime;

	if (enemyComp->DeadTime >= timeToDelete)
	{
		enemyComp->SceneManager.lock()->DeleteEntity(enemyComp->GetEntityID());
		Log::GetClientLogger()->info("Destroy Enemy");
	}
}

void EnemyDeadState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit DeadState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}
