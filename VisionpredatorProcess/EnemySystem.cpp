#include "pch.h"
#include "EnemySystem.h"
#include "VisPredComponents.h"
EnemySystem::EnemySystem(std::shared_ptr<SceneManager> SceneMagener):System(SceneMagener)
{
}
void EnemySystem::Update(float deltaTime)
{
}

void EnemySystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(EnemyComponent, enemycomp)
	{
		Calculate_FSM(enemycomp);
		Action_FSM(enemycomp);
	}
}
#pragma region FSM Calculate
void EnemySystem::Calculate_FSM(EnemyComponent& enemycomp)
{
	switch (enemycomp.CurrentFSM)
	{
	case VisPred::Game::EFSM::IDLE:
		Calculate_Idle(enemycomp);
		break;
	case VisPred::Game::EFSM::MOVE:
		Calculate_Move(enemycomp);
		break;
	case VisPred::Game::EFSM::ATTACK:
		Calculate_Attack(enemycomp);
		break;
	case VisPred::Game::EFSM::DIE:
		Calculate_Die(enemycomp);
		break;
	case VisPred::Game::EFSM::DESTROY:
		Calculate_Destroy(enemycomp);
		break;
	default:
		break;
	}

}
void EnemySystem::Calculate_Idle(EnemyComponent& enemycomp)
{
	if (enemycomp.HP <= 0)
		enemycomp.CurrentFSM = VisPred::Game::EFSM::DIE;
	else if (1 == 2)
		enemycomp.CurrentFSM = VisPred::Game::EFSM::ATTACK;
	else if (1 == 2)
		enemycomp.CurrentFSM = VisPred::Game::EFSM::MOVE;
}
void EnemySystem::Calculate_Die(EnemyComponent& enemycomp)
{
	///애니메이션이 끝나면 DIE-> DESTROY로 변경이 되어야 한다.
	if (1 == 2)
		enemycomp.CurrentFSM = VisPred::Game::EFSM::DESTROY;
}
void EnemySystem::Calculate_Attack(EnemyComponent& enemycomp)
{
	///애니메이션이 끝나면 DIE-> DESTROY로 변경이 되어야 한다.
	if (enemycomp.HP <= 0)
		enemycomp.CurrentFSM = VisPred::Game::EFSM::DIE;
	else if (1 == 2)
		enemycomp.CurrentFSM = VisPred::Game::EFSM::MOVE;
}
void EnemySystem::Calculate_Move(EnemyComponent& enemycomp)
{
	///애니메이션이 끝나면 DIE-> DESTROY로 변경이 되어야 한다.
	if (enemycomp.HP<=0)
		enemycomp.CurrentFSM = VisPred::Game::EFSM::DIE;
	else if (1 == 2)
		enemycomp.CurrentFSM = VisPred::Game::EFSM::ATTACK;
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
	case VisPred::Game::EFSM::IDLE:
		Action_Idle(enemycomp);
		break;
	case VisPred::Game::EFSM::MOVE:
		Action_Move(enemycomp);
		break;
	case VisPred::Game::EFSM::ATTACK:
		Action_Attack(enemycomp);

		break;
	case VisPred::Game::EFSM::DIE:
		Action_Die(enemycomp);
		break;
	case VisPred::Game::EFSM::DESTROY:
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
