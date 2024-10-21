#include "pch.h"
#include "EnemyIdleState.h"
#include "EnemyComponent.h"
#include "StatesInclude.h"
#include "TransformComponent.h"
#include "SceneManager.h"

void EnemyIdleState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter IdleState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	ChangeCurrentState(enemyComp, &EnemyMovementState::s_Idle);
}

void EnemyIdleState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	if (CheckIsDead(enemyComp))
		return;
	DetectTarget(*enemyComp, deltaTime);

	auto transform = enemyComp->GetComponent<TransformComponent>();
	auto pos = transform->World_Location;
	auto rotation = transform->World_Rotation;

	static float tempTime = 0.f;

	tempTime += deltaTime;

	if (tempTime >= 2.f)
	{
		//enemyComp->SceneManager.lock()->SpawnEditablePrefab("../Data/Prefab/Pistol_Bullet.prefab", pos, rotation);
		enemyComp->SceneManager.lock()->SpawnEditablePrefab("../Data/Prefab/Sound_Pistol.prefab", pos, rotation);
		tempTime = 0.f;
	}
}

void EnemyIdleState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit IdleState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}
