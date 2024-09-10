#include "pch.h"
#include "NavAgentSystem.h"
#include <iostream>

NavAgentSystem::NavAgentSystem(std::shared_ptr<SceneManager> sceneManager):System{ sceneManager }
{
}


void NavAgentSystem::SetSpeed(uint32_t entityid, float speed)
{
	if (!GetSceneManager()->HasComponent<NavAgentComponent>(entityid))
		return;
	SetSpeed(GetSceneManager()->GetComponent<NavAgentComponent>(entityid), speed);
}
void NavAgentSystem::SetRadius(uint32_t entityid, float radius)
{
	if (!GetSceneManager()->HasComponent<NavAgentComponent>(entityid))
		return;
	SetRadius(GetSceneManager()->GetComponent<NavAgentComponent>(entityid), radius);
}
void NavAgentSystem::SetAcceleration(uint32_t entityid, float accel)
{
	if (!GetSceneManager()->HasComponent<NavAgentComponent>(entityid))
		return;
	SetAcceleration(GetSceneManager()->GetComponent<NavAgentComponent>(entityid), accel);
}
void NavAgentSystem::SetHegiht(uint32_t entityid, float Height)
{
	if (!GetSceneManager()->HasComponent<NavAgentComponent>(entityid))
		return;
	SetHegiht(GetSceneManager()->GetComponent<NavAgentComponent>(entityid), Height);
}

void NavAgentSystem::SetSpeed(NavAgentComponent* comp, float speed)
{

	comp->NavAgent->agentParams.maxSpeed = speed;
	if (!GetSceneManager()->GetSceneNavMeshData())
		return;
	auto SceneNavMeshData = GetSceneManager()->GetSceneNavMeshData();
	if (SceneNavMeshData->crowd != nullptr)
		SceneNavMeshData->crowd->updateAgentParameters(comp->GetEntityID(), &comp->NavAgent->agentParams);
}

void NavAgentSystem::SetAcceleration(NavAgentComponent* comp, float accel)
{
	comp->NavAgent->agentParams.maxAcceleration = accel;
	if (!GetSceneManager()->GetSceneNavMeshData())
		return;
	auto SceneNavMeshData = GetSceneManager()->GetSceneNavMeshData();
	if (SceneNavMeshData->crowd != nullptr)
		SceneNavMeshData->crowd->updateAgentParameters(comp->GetEntityID(), &comp->NavAgent->agentParams);
}

void NavAgentSystem::SetRadius(NavAgentComponent* comp, float radius)
{
	comp->NavAgent->agentParams.radius = radius;
	if (!GetSceneManager()->GetSceneNavMeshData())
		return;
	auto SceneNavMeshData = GetSceneManager()->GetSceneNavMeshData();
	if (SceneNavMeshData->crowd != nullptr)
		SceneNavMeshData->crowd->updateAgentParameters(comp->GetEntityID(), &comp->NavAgent->agentParams);
}

void NavAgentSystem::SetHegiht(NavAgentComponent* comp, float height)
{
	comp->NavAgent->agentParams.height = height;
	if (!GetSceneManager()->GetSceneNavMeshData())
		return;
	auto SceneNavMeshData = GetSceneManager()->GetSceneNavMeshData();
	if (SceneNavMeshData->crowd != nullptr)
		SceneNavMeshData->crowd->updateAgentParameters(comp->GetEntityID(), &comp->NavAgent->agentParams);
}


void NavAgentSystem::MoveTo(NavAgentComponent* comp, VPMath::Vector3 destination)
{
	auto SceneNavMeshData = GetSceneManager()->GetSceneNavMeshData();
	const dtQueryFilter* filter = SceneNavMeshData->crowd->getFilter(0);
	const dtCrowdAgent* agent = SceneNavMeshData->crowd->getAgent(comp->NavAgent->AgentID);  // 오타 수정
	const float* halfExtents = SceneNavMeshData->crowd->getQueryExtents();

	// destination이 VPMath::Vector3에서 안전하게 float*로 변환되는지 확인
	dtPolyRef nearestPoly;
	dtStatus status = SceneNavMeshData->navQuery->findNearestPoly(reinterpret_cast<float*>(&destination), halfExtents, filter, &comp->NavAgent->targetRef, comp->NavAgent->targetPos);

	//if (dtStatusFailed(status)) {
	//	std::cerr << "Failed to find nearest poly for destination: " << destination.x << ", " << destination.y << ", " << destination.z << std::endl;
	//	return;
	//}

	//std::cout << "Target Pos after: " << comp->NavAgent->targetPos[0] << ", " << comp->NavAgent->targetPos[1] << ", " << comp->NavAgent->targetPos[2] << std::endl;

	bool check = SceneNavMeshData->crowd->requestMoveTarget(comp->NavAgent->AgentID, comp->NavAgent->targetRef, comp->NavAgent->targetPos);

	//// requestMoveTarget의 반환값 처리
	//if (!check) {
	//	// 오류 처리 또는 경고 로그 추가
	//	std::cerr << "Move target request failed for agent ID: " << comp->NavAgent->AgentID << std::endl;
	//	return;  // 실패 시 함수 종료
	//}

	comp->NavAgent->IsStop = false;
}

void NavAgentSystem::Stop(NavAgentComponent* comp)
{
	auto navmeshdata= GetSceneManager()->GetSceneNavMeshData();
	navmeshdata->crowd->resetMoveTarget(comp->GetEntityID());
	comp->NavAgent->IsStop= true;
}
void NavAgentSystem::Update(float deltaTime)
{
	COMPLOOP(NavAgentComponent, navcomp)
	{
		if (!navcomp.IsChase)
			continue;

		COMPLOOP(IdentityComponent, identitycomp)
		{
			if (identitycomp.UUID == navcomp.TargetName)
			{
				auto transform = identitycomp.GetComponent<TransformComponent>();
				MoveTo(&navcomp, transform->World_Location);
				break;
			}
		}
		
	}

}

void NavAgentSystem::AssignToNavigationField(NavAgentComponent* comp)
{
	//comp->NavAgnet->AgentID= Get
	comp->NavAgent->AgentID=this->AddAgentToCrowd(comp);
}

int NavAgentSystem::AddAgentToCrowd(NavAgentComponent* comp)
{
	auto transform = comp->GetComponent<TransformComponent>();

	const float posf[3] = { transform->World_Location.x, transform->World_Location.y, transform->World_Location.z };
	return GetSceneManager()->GetSceneNavMeshData()->crowd->addAgent(posf, &comp->NavAgent->agentParams);
}

void NavAgentSystem::Initialize()
{
	COMPLOOP(NavAgentComponent, navcomp)
	{
		Start(navcomp.GetEntityID());
	}

}

void NavAgentSystem::Start(uint32_t gameObjectId)
{
	if (!GetSceneManager()->HasComponent<NavAgentComponent>(gameObjectId))
		return;
	if (GetSceneManager()->HasComponent<ControllerComponent>(gameObjectId))
	{
		const auto& controller = GetSceneManager()->GetComponent<ControllerComponent>(gameObjectId);
		auto agentcomp = controller->GetComponent<NavAgentComponent>();
		agentcomp->NavAgent = new NavAgentData();
		this->SetAcceleration(agentcomp, controller->Acceleration);
		this->SetRadius(agentcomp, controller->CapsuleControllerinfo.radius);
		this->SetSpeed(agentcomp, controller->MaxSpeed);
		SetHegiht(agentcomp, (controller->CapsuleControllerinfo.height + controller->CapsuleControllerinfo.radius));
		this->AssignToNavigationField(agentcomp);
		return;

	}
	else
	{

		const auto& transformcomp = GetSceneManager()->GetComponent<TransformComponent>(gameObjectId);
		auto agentcomp = transformcomp->GetComponent<NavAgentComponent>();
		agentcomp->NavAgent = new NavAgentData();
		this->SetAcceleration(agentcomp, 10 );
		this->SetRadius(agentcomp, 2);
		this->SetHegiht(agentcomp, 3);
		this->SetSpeed(agentcomp,7);
		this->AssignToNavigationField(agentcomp);
		return;

	}




}

void NavAgentSystem::Finish(uint32_t gameObjectId)
{
	if (!GetSceneManager()->HasComponent<NavAgentComponent>(gameObjectId))
		return;
	auto agentcomp = GetSceneManager()->GetComponent<NavAgentComponent>(gameObjectId);
	auto navmeshdata = GetSceneManager()->GetSceneNavMeshData();
	if (navmeshdata)
		navmeshdata->crowd->removeAgent(agentcomp->NavAgent->AgentID);
	delete agentcomp->NavAgent;
}

void NavAgentSystem::Finalize()
{
	COMPLOOP(NavAgentComponent, comp)
	{
		Finish(comp.GetEntityID());
	}
}
