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
		DetectTarget(enemycomp, deltaTime);
		Calculate_FSM(enemycomp);
		Action_FSM(enemycomp);
	}
}

#pragma region FSM Calculate
void EnemySystem::Calculate_FSM(EnemyComponent& enemycomp)
{
	// 하위 State 들을 만드는데.
	// 이 State 들을 컨테이너에 넣어놓고
	// 각 상위 State 들 마다 이것들을 확인하여 그에 맞는 행동을 한다.
	// ex) 

	switch (enemycomp.CurrentFSM)
	{
		case VisPred::Game::EnemyState::Idle:
			Calculate_Idle(enemycomp);
			break;
		case VisPred::Game::EnemyState::Chase:
			Calculate_Chase(enemycomp);
			break;
		case VisPred::Game::EnemyState::Patrol:
			Calculate_Patrol(enemycomp);
			break;
		case VisPred::Game::EnemyState::Dead:
			Calculate_Die(enemycomp);
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
	//else if (1 == 2)
	//	enemycomp.CurrentFSM = VisPred::Game::EnemyState::ATTACK;

	//enemycomp.CurrentFSM = VisPred::Game::EnemyState::WALK;

	// TODO: 애니메이션 상태 변경
}

void EnemySystem::Calculate_Chase(EnemyComponent& enemycomp)
{
	// 플에이어 추격 On
	uint32_t enemyID = enemycomp.GetEntityID();
	if (true == m_SceneManager.lock()->HasComponent<NavAgentComponent>(enemyID))
	{
		auto navComp = m_SceneManager.lock()->GetComponent<NavAgentComponent>(enemyID);
		navComp->IsChase = true;
	}

	if (enemycomp.HP <= 0)
	{
		Die(enemycomp);
	}

	// TODO: 하위 상태에 따라 애니메이션 상태 변경
}

void EnemySystem::Calculate_Patrol(EnemyComponent& enemycomp)
{
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
	//if (1 == 2)
	//	enemycomp.CurrentFSM = VisPred::Game::EnemyState::DESTROY;
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
		case VisPred::Game::EnemyState::Idle:
			Action_Idle(enemycomp);
			break;
			//case VisPred::Game::EnemyState::WALK:
			//	Action_Move(enemycomp);
			//break;
		//case VisPred::Game::EnemyState::ATTACK:
		//	Action_Attack(enemycomp);
			//break;
		case VisPred::Game::EnemyState::Dead:
			Action_Die(enemycomp);
			break;
			//case VisPred::Game::EnemyState::DESTROY:
			//	Action_Destroy(enemycomp);
			//	break;
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
	enemycomp.CurrentFSM = VisPred::Game::EnemyState::Dead;
	uint32_t id = enemycomp.GetEntityID();
	int aniIndex = static_cast<int>(enemycomp.CurrentFSM); //해당 인덱스 enum으로 뽑아서 일관적으로 써야할듯
	VisPred::Engine::AniBlendData temp{ enemycomp.GetEntityID() ,aniIndex ,2,false };
	std::any data = temp;
	EventManager::GetInstance().ScheduleEvent("OnChangeAnimation", data);

	if (enemycomp.HasComponent<AnimationComponent>())
	{
		auto ani = enemycomp.GetComponent<AnimationComponent>();
		ani->isLoop = false;
	}
}


float WrapAngle(float angle)
{
	while (angle > 180.0f) angle -= 360.0f;
	while (angle < -180.0f) angle += 360.0f;
	return angle;
}

void EnemySystem::DetectTarget(EnemyComponent& enemyComp, float deltaTime)
{
	uint32_t enemyID = enemyComp.GetEntityID();
	auto transform = enemyComp.GetComponent<TransformComponent>();

	VisPred::SimpleMath::Vector3 enemyPos =
	{ transform->WorldTransform._41, transform->WorldTransform._42 + 1.8f, transform->WorldTransform._43 };

	// 시야 범위 생성
	DirectX::BoundingFrustum frustum;
	D3DUtill::CreateBoundingFrustum(
		enemyPos,
		transform->FrontVector,
		transform->UpVector,
		enemyComp.HorizontalFOV,
		enemyComp.VerticalFOV,
		enemyComp.NearZ,
		enemyComp.FarZ,
		frustum,
		enemyComp.IsModelFlipped
	);

	// Draw frustum debug draw
#ifdef _DEBUG 
	debug::FrustumInfo frustumInfo;
	frustumInfo.Frustum = frustum;
	frustumInfo.Color = VisPred::SimpleMath::Color{ 1, 1, 0, 1 };
	m_Graphics->DrawFrustum(frustumInfo);
#endif _DEBUG

	uint32_t playerID;
	VisPred::SimpleMath::Vector3 playerPos;
	COMPLOOP(PlayerComponent, comp)
	{
		playerID = comp.GetEntityID();
		auto playerTransform = m_SceneManager.lock()->GetComponent<TransformComponent>(playerID);

		playerPos = VisPred::SimpleMath::Vector3{ playerTransform->WorldTransform._41, playerTransform->WorldTransform._42, playerTransform->WorldTransform._43 };
	}

	// 플레이어가 Enemy의 시야 범위 안에 들어와있을 때에만 레이캐스트 수행
	bool isPlayerInSight = false;
	isPlayerInSight = frustum.Contains(playerPos);
	if (true == isPlayerInSight)
	{
		auto targetDir = playerPos - enemyPos;
		targetDir.Normalize();

		float distanceToPlayer = targetDir.Length();

		VisPred::SimpleMath::Vector3 targetDirTemp = targetDir;
		targetDirTemp.y = 0; // Y값 고정

		auto targetRotation  = VPMath::Quaternion::LookRotation(targetDirTemp, VPMath::Vector3::Up);

		auto enemyWorldQuaternion = transform->World_Quaternion;
		// Slerp로 부드럽게 회전 (t는 회전 속도를 제어)
		// 거리 기반 회전 속도 조정 (가까울수록 빠르게 회전)
		float baseRotationSpeed = 1.5f;  // 기본 회전 속도
		float rotationSpeed = baseRotationSpeed + (1.0f / distanceToPlayer) * 10.0f;  // 거리에 비례해 회전 속도 증가

		auto newRotation = VPMath::Quaternion::Slerp(enemyWorldQuaternion, targetRotation, deltaTime * rotationSpeed);

		transform->SetWorldQuaternion(newRotation);
		//transform->SetWorldQuaternion(targetRotation);

		//uint32_t detectedObjID = m_PhysicsEngine->RaycastToHitActor(enemyID, targetDir, enemyComp.FarZ);
		uint32_t detectedObjID = m_PhysicsEngine->RaycastToHitActorFromLocation_Ignore(enemyID, enemyPos, targetDir, enemyComp.FarZ);
		if (detectedObjID == playerID)
			enemyComp.CurrentFSM = VisPred::Game::EnemyState::Chase;
		else
		{
			/// TEMP
			enemyComp.CurrentFSM = VisPred::Game::EnemyState::Idle;
		}
	}
	else
	{
		/// TEMP
		enemyComp.CurrentFSM = VisPred::Game::EnemyState::Idle;
	}

}

void EnemySystem::PhysicsUpdate(float deltaTime)
{
}

void EnemySystem::PhysicsLateUpdate(float deltaTime)
{
}

