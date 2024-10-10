#include "pch.h"
#include "EnemySystem.h"
#include "VisPredComponents.h"
#include "EngineStructs.h"
#include "../VPGraphics/D3DUtill.h"
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
		Calculate_FSM(enemycomp);
		Action_FSM(enemycomp);

		DetectTarget(enemycomp);
	}
}
#pragma region FSM Calculate
void EnemySystem::Calculate_FSM(EnemyComponent& enemycomp)
{
	switch (enemycomp.CurrentFSM)
	{
		case VisPred::Game::EnemyState::ATTACK:
			Calculate_Attack(enemycomp);
			break;
		case VisPred::Game::EnemyState::IDLE:
			Calculate_Idle(enemycomp);
			break;
		case VisPred::Game::EnemyState::CHASE:
			Calculate_Chase(enemycomp);
			break;
		case VisPred::Game::EnemyState::DIE:
			Calculate_Die(enemycomp);
			break;
		case VisPred::Game::EnemyState::ATTACKED:
			Calculate_Attacked(enemycomp);
			break;
		case VisPred::Game::EnemyState::BACKWALK:
			Calculate_BackWalk(enemycomp);
			break;
		case VisPred::Game::EnemyState::DESTROY:
			Calculate_Destroy(enemycomp);
			break;
		default:
			break;
	}

}

void EnemySystem::Calculate_Attack(EnemyComponent& enemycomp)
{
	///애니메이션이 끝나면 DIE-> DESTROY로 변경이 되어야 한다.
	if (enemycomp.HP <= 0)
	{
		Die(enemycomp);
	}
	else if (1 == 2)
		;
	//enemycomp.CurrentFSM = VisPred::Game::EnemyState::WALK;
}

void EnemySystem::Calculate_Idle(EnemyComponent& enemycomp)
{
	/// OnStart -------------------------------------------------------------------------------------
	// 플에이어 추격 종료
	uint32_t enemyID = enemycomp.GetEntityID();
	if (true == m_SceneManager.lock()->HasComponent<NavAgentComponent>(enemyID))
	{
		auto navComp = m_SceneManager.lock()->GetComponent<NavAgentComponent>(enemyID);
		navComp->IsChase = false;
	}

	if (enemycomp.HP <= 0)
	{
		Die(enemycomp);
		m_PhysicsEngine->RemoveController(enemycomp.GetEntityID());
	}
	else if (1 == 2)
		enemycomp.CurrentFSM = VisPred::Game::EnemyState::ATTACK;

	//enemycomp.CurrentFSM = VisPred::Game::EnemyState::WALK;
}

void EnemySystem::Calculate_Chase(EnemyComponent& enemycomp)
{
	if (enemycomp.HP <= 0)
	{
		Die(enemycomp);
	}
}

void EnemySystem::Calculate_Walk(EnemyComponent& enemycomp)
{
	if (enemycomp.HP <= 0)
	{
		Die(enemycomp);
	}
}

void EnemySystem::Calculate_Die(EnemyComponent& enemycomp)
{
	///애니메이션이 끝나면 DIE-> DESTROY로 변경이 되어야 한다.
	if (1 == 2)
		enemycomp.CurrentFSM = VisPred::Game::EnemyState::DESTROY;
}

void EnemySystem::Calculate_Attacked(EnemyComponent& enemycomp)
{
	if (enemycomp.HP <= 0)
	{
		Die(enemycomp);
	}
}

void EnemySystem::Calculate_BackWalk(EnemyComponent& enemycomp)
{
	if (enemycomp.HP <= 0)
	{
		Die(enemycomp);
	}
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

void EnemySystem::Die(EnemyComponent& enemycomp)
{
	enemycomp.CurrentFSM = VisPred::Game::EnemyState::DIE;
	uint32_t id = enemycomp.GetEntityID();
	int aniIndex = static_cast<int>(enemycomp.CurrentFSM); //해당 인덱스 enum으로 뽑아서 일관적으로 써야할듯
	VisPred::Engine::AniBlendData temp{ enemycomp.GetEntityID() ,aniIndex ,2,false};
	std::any data = temp;
	EventManager::GetInstance().ScheduleEvent("OnChangeAnimation", data);

	if (enemycomp.HasComponent<AnimationComponent>())
	{
		auto ani = enemycomp.GetComponent<AnimationComponent>();
		ani->isLoop = false;
	}
}

void EnemySystem::DetectTarget(EnemyComponent& enemycomp)
{
	auto transform = enemycomp.GetComponent<TransformComponent>();

	VisPred::SimpleMath::Vector3 enemyPos =
	{ transform->WorldTransform._41, transform->WorldTransform._42 + 1.8f, transform->WorldTransform._43 };

	// 시야 범위 생성
	DirectX::BoundingFrustum frustum;
	D3DUtill::CreateBoundingFrustum(
		enemyPos,
		transform->FrontVector,
		transform->UpVector,
		60.0f,
		40.0f,
		0.2f,
		13.0f,
		frustum,
		true
	);

#ifdef _DEBUG
	debug::FrustumInfo frustumInfo;
	frustumInfo.Frustum = frustum;
	frustumInfo.Color = VisPred::SimpleMath::Color{ 1, 1, 0, 1 };
	m_Graphics->DrawFrustum(frustumInfo);
#endif _DEBUG

	VisPred::SimpleMath::Vector3 playerPos;
	COMPLOOP(PlayerComponent, comp)
	{
		uint32_t playerID = comp.GetEntityID();
		auto playerTransform = m_SceneManager.lock()->GetComponent<TransformComponent>(playerID);

		playerPos = VisPred::SimpleMath::Vector3{ playerTransform->WorldTransform._41, playerTransform->WorldTransform._42, playerTransform->WorldTransform._43 };
	}

	bool isPlayerInSight = false;
	isPlayerInSight = frustum.Contains(playerPos);
}

void EnemySystem::PhysicsUpdate(float deltaTime)
{
}

void EnemySystem::PhysicsLateUpdate(float deltaTime)
{
}

