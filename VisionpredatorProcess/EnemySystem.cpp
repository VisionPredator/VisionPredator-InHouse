#include "pch.h"
#include "EnemySystem.h"
#include "VisPredComponents.h"
EnemySystem::EnemySystem(std::shared_ptr<SceneManager> SceneMagener) :System(SceneMagener)
{
}
void EnemySystem::Update(float deltaTime)
{
}

void EnemySystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(EnemyComponent, enemycomp)
	{
		if (INPUTKEYDOWN(KEYBOARDKEY::H))
		{
			enemycomp.HP = -1;
		}

		Calculate_FSM(enemycomp);
		Action_FSM(enemycomp);
	}
}
#pragma region FSM Calculate
void EnemySystem::Calculate_FSM(EnemyComponent& enemycomp)
{
	switch (enemycomp.CurrentFSM)
	{
		case VisPred::Game::EnemyState::IDLE:
			Calculate_Idle(enemycomp);
			break;
		case VisPred::Game::EnemyState::CHASE:
			Calculate_Move(enemycomp);
			break;
		case VisPred::Game::EnemyState::ATTACK:
			Calculate_Attack(enemycomp);
			break;
		case VisPred::Game::EnemyState::DIE:
			Calculate_Die(enemycomp);
			break;
		case VisPred::Game::EnemyState::DESTROY:
			Calculate_Destroy(enemycomp);
			break;
		default:
			break;
	}

}
void EnemySystem::Calculate_Idle(EnemyComponent& enemycomp)
{
	if (enemycomp.HP <= 0)
	{
		enemycomp.CurrentFSM = VisPred::Game::EnemyState::DIE;
		uint32_t id = enemycomp.GetEntityID();
		int aniIndex = 3; //해당 인덱스 enum으로 뽑아서 일관적으로 써야할듯
		EventManager::GetInstance().ScheduleEvent("OnChangeAnimation", std::pair<uint32_t, int>(id, aniIndex));

	}
	else if (1 == 2)
		enemycomp.CurrentFSM = VisPred::Game::EnemyState::ATTACK;
	else if (1 == 2)
		;

	//enemycomp.CurrentFSM = VisPred::Game::EnemyState::WALK;
}
void EnemySystem::Calculate_Die(EnemyComponent& enemycomp)
{
	///애니메이션이 끝나면 DIE-> DESTROY로 변경이 되어야 한다.
	if (1 == 2)
		enemycomp.CurrentFSM = VisPred::Game::EnemyState::DESTROY;
}
void EnemySystem::Calculate_Attack(EnemyComponent& enemycomp)
{
	///애니메이션이 끝나면 DIE-> DESTROY로 변경이 되어야 한다.
	if (enemycomp.HP <= 0)
		enemycomp.CurrentFSM = VisPred::Game::EnemyState::DIE;
	else if (1 == 2)
		;
	//enemycomp.CurrentFSM = VisPred::Game::EnemyState::WALK;
}
void EnemySystem::Calculate_Move(EnemyComponent& enemycomp)
{
	///애니메이션이 끝나면 DIE-> DESTROY로 변경이 되어야 한다.
	if (enemycomp.HP <= 0)
		enemycomp.CurrentFSM = VisPred::Game::EnemyState::DIE;
	else if (1 == 2)
		enemycomp.CurrentFSM = VisPred::Game::EnemyState::ATTACK;
}
void EnemySystem::Calculate_Destroy(EnemyComponent& enemycomp)
{

}
#pragma endregion 

#pragma region FSM Action
void EnemySystem::Action_FSM(EnemyComponent& enemycomp)
{
	switch (enemycomp.CurrentFSM)
	{
		case VisPred::Game::EnemyState::IDLE:
			Action_Idle(enemycomp);
			break;
			//case VisPred::Game::EnemyState::WALK:
			//	Action_Move(enemycomp);
			break;
		case VisPred::Game::EnemyState::ATTACK:
			Action_Attack(enemycomp);

			break;
		case VisPred::Game::EnemyState::DIE:
			Action_Die(enemycomp);
			break;
		case VisPred::Game::EnemyState::DESTROY:
			Action_Destroy(enemycomp);
			break;
		default:
			break;
	}

}
void EnemySystem::Action_Idle(EnemyComponent& enemycomp)
{


}

void EnemySystem::Action_Move(EnemyComponent& enemycomp)
{
	if (enemycomp.HasComponent<NavAgentComponent>())
	{
		auto asd = enemycomp.GetComponent<NavAgentComponent>();
		asd->IsChase = true; // 움직인다
		asd->TargetName = "asd"; //
	}




}

void EnemySystem::Action_Attack(EnemyComponent& enemycomp)
{

}

void EnemySystem::Action_Die(EnemyComponent& enemycomp)
{
}
void EnemySystem::Action_Destroy(EnemyComponent& enemycomp)
{
	GetSceneManager()->DestroyEntity(enemycomp.GetEntityID());
}
#pragma endregion

#pragma region GunShoot
void EnemySystem::Shoot_Style(EnemyComponent& enemycomp)
{
	switch (enemycomp.EnemyType)
	{
		case VisPred::Game::GunType::PISTOL:
			Shoot_Pistol(enemycomp);
			break;
		case VisPred::Game::GunType::SHOTGUN:
			Shoot_ShotGun(enemycomp);
			break;
		case VisPred::Game::GunType::RIFLE:
			Shoot_Rifle(enemycomp);
			break;
		default:
			break;
	}

}

void EnemySystem::Shoot_Pistol(EnemyComponent& enemycomp)
{

}

void EnemySystem::Shoot_ShotGun(EnemyComponent& enemycomp)
{
}

void EnemySystem::Shoot_Rifle(EnemyComponent& enemycomp)
{
}

#pragma endregion
