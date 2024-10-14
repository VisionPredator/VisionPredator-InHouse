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
	// ���� State ���� ����µ�.
	// �� State ���� �����̳ʿ� �־����
	// �� ���� State �� ���� �̰͵��� Ȯ���Ͽ� �׿� �´� �ൿ�� �Ѵ�.
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
	///�ִϸ��̼��� ������ DIE-> DESTROY�� ������ �Ǿ�� �Ѵ�.
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
	// �ÿ��̾� �߰� ����
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

	// TODO: �ִϸ��̼� ���� ����
}

void EnemySystem::Calculate_Chase(EnemyComponent& enemycomp)
{
	// �ÿ��̾� �߰� On
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

	// TODO: ���� ���¿� ���� �ִϸ��̼� ���� ����
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
	///�ִϸ��̼��� ������ DIE-> DESTROY�� ������ �Ǿ�� �Ѵ�.
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
		asd->IsChase = true; // �����δ�
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
	int aniIndex = static_cast<int>(enemycomp.CurrentFSM); //�ش� �ε��� enum���� �̾Ƽ� �ϰ������� ����ҵ�
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

	// �þ� ���� ����
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

	// �÷��̾ Enemy�� �þ� ���� �ȿ� �������� ������ ����ĳ��Ʈ ����
	bool isPlayerInSight = false;
	isPlayerInSight = frustum.Contains(playerPos);
	if (true == isPlayerInSight)
	{
		auto targetDir = playerPos - enemyPos;
		targetDir.Normalize();

		float distanceToPlayer = targetDir.Length();

		VisPred::SimpleMath::Vector3 targetDirTemp = targetDir;
		targetDirTemp.y = 0; // Y�� ����

		auto targetRotation  = VPMath::Quaternion::LookRotation(targetDirTemp, VPMath::Vector3::Up);

		auto enemyWorldQuaternion = transform->World_Quaternion;
		// Slerp�� �ε巴�� ȸ�� (t�� ȸ�� �ӵ��� ����)
		// �Ÿ� ��� ȸ�� �ӵ� ���� (�������� ������ ȸ��)
		float baseRotationSpeed = 1.5f;  // �⺻ ȸ�� �ӵ�
		float rotationSpeed = baseRotationSpeed + (1.0f / distanceToPlayer) * 10.0f;  // �Ÿ��� ����� ȸ�� �ӵ� ����

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

