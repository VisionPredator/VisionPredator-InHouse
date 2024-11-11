#include "pch.h"
#include "EnemyDeadState.h"
#include "EnemyComponent.h"
#include "EnemySoundComponent.h"
#include "RigidBodyComponent.h"
#include "StatesInclude.h"
#include "EventManager.h"
#include "../VPEngine/SceneManager.h"
#include <random>

void EnemyDeadState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter DeadState");
	const auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	enemyComp->CurrentFSM = VisPred::Game::EnemyStates::Dead;

	if (enemyComp->HasComponent<ControllerComponent>())
	{
		enemyComp->PhysicsManager->RemoveController(enemyComp->GetEntityID());
	}
	EventManager::GetInstance().ImmediateEvent("OnRemoveNavAgent", enemyComp->GetEntityID());

	const auto decalComp = enemyComp->SceneManager.lock()->GetChildEntityComp_HasComp<DecalComponent>(enemyComp->GetEntityID());

	// 데칼 그림자 삭제
	if (decalComp)
	{
		enemyComp->SceneManager.lock()->DestroyEntity(decalComp->GetEntityID());
	}

	// 적이 들고 있는 무기를 바닥에 드랍.
	auto dropWeaponPos = enemyComp->GetComponent<TransformComponent>()->World_Location;
	dropWeaponPos.y += 1.f;
	std::shared_ptr<Entity> weapon;
	switch (enemyComp->EnemyType)
	{
	case VisPred::Game::GunType::PISTOL:
	{
		weapon = enemyComp->SceneManager.lock()->SpawnEditablePrefab("../Data/Prefab/Pistol.prefab", dropWeaponPos, VPMath::Vector3{}, { .2f,.2f,.2f });
	}
	break;
	case VisPred::Game::GunType::SHOTGUN:
	{
		weapon = enemyComp->SceneManager.lock()->SpawnEditablePrefab("../Data/Prefab/ShotGun.prefab", dropWeaponPos, VPMath::Vector3{}, { .2f,.2f,.2f });
	}
	break;
	case VisPred::Game::GunType::RIFLE:
	{
		weapon = enemyComp->SceneManager.lock()->SpawnEditablePrefab("../Data/Prefab/Rifle.prefab", dropWeaponPos, VPMath::Vector3{}, { .2f,.2f,.2f });
	}
	break;
	default:
		break;
	}
	if (weapon)
		EventManager::GetInstance().ScheduleEvent("OnAddVelecity", std::make_tuple(weapon->GetEntityID(), VPMath::Vector3{ 0.f,1.f,0.f }, 2.f));

	// 난수 생성기 초기화
	std::random_device rd;  // 하드웨어 엔트로피를 기반으로 시드 생성
	std::mt19937 gen(rd()); // Mersenne Twister 난수 생성기

	const auto soundComp = enemyComp->GetComponent<EnemySoundComponent>();
	std::string soundKey{};
	int volume{};

	// 세가지 사망 사운드중 랜덤으로 하나를 고른다.
	switch (std::uniform_int_distribution dist(1, 3); dist(gen))
	{
	case 1:
		{
			soundKey = soundComp->SoundKey_Death1;
			volume = soundComp->Volume_Death1;
		}
		break;
	case 2:
		{
			soundKey = soundComp->SoundKey_Death2;
			volume = soundComp->Volume_Death2;
		}
		break;
	case 3:
		{
			soundKey = soundComp->SoundKey_Death3;
			volume = soundComp->Volume_Death3;
		}
		break;
	default: ;
	}

	// 사망 사운드 출력
	enemyComp->SceneManager.lock()->SpawnSoundEntity(
		soundKey,
		volume,
		false,
		false,
		enemyComp->GetComponent<TransformComponent>()->World_Location
	);

	ChangeCurrentState(enemyComp, &EnemyCombatState::s_Idle);
	ChangeCurrentState(enemyComp, &EnemyMovementState::s_Idle);
	enemyComp->MovementState->Enter(enemyComp);
	EventManager::GetInstance().ImmediateEvent("OnEnemyKilled", enemyComp->GetEntityID());
	EventManager::GetInstance().ImmediateEvent("OnUpdateHP");
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
