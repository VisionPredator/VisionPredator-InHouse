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
	comp->NavAgent->agentParams.collisionQueryRange = radius*2+1;
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
	if (comp->TargetLocation == destination)
		return; 
	comp->TargetLocation = destination;

	auto SceneNavMeshData = GetSceneManager()->GetSceneNavMeshData();
	const dtQueryFilter* filter = SceneNavMeshData->crowd->getFilter(0);
	const dtCrowdAgent* agent = SceneNavMeshData->crowd->getAgent(comp->NavAgent->AgentID);  // 오타 수정
	const float* halfExtents = SceneNavMeshData->crowd->getQueryExtents();

	SceneNavMeshData->navQuery->findNearestPoly(reinterpret_cast<float*>(&destination), halfExtents, filter, &comp->NavAgent->targetRef, comp->NavAgent->targetPos);

	bool temp = SceneNavMeshData->crowd->requestMoveTarget(comp->NavAgent->AgentID, comp->NavAgent->targetRef, comp->NavAgent->targetPos);


	comp->NavAgent->IsStop = false;
}

void NavAgentSystem::Stop(NavAgentComponent* comp)
{
	auto navmeshdata = GetSceneManager()->GetSceneNavMeshData();
	if (!navmeshdata)
		return;
	if (!navmeshdata->crowd)
		return;
	navmeshdata->crowd->resetMoveTarget(comp->NavAgent->AgentID);
	comp->TargetLocation = {};

	comp->NavAgent->IsStop = true;
}
void NavAgentSystem::PhysicsUpdate(float deltaTime)
{
	COMPLOOP(NavAgentComponent, navcomp)
	{
		if (!navcomp.NavAgent)
			continue;

		if (!navcomp.IsChase)
		{
			if (!navcomp.NavAgent->IsStop)
				Stop(&navcomp);


			continue;
		}


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


void NavAgentSystem::AddAgentToCrowd(NavAgentComponent* comp)
{
	auto transform = comp->GetComponent<TransformComponent>();

	const float posf[3] = { transform->World_Location.x, transform->World_Location.y, transform->World_Location.z };
	if (!GetSceneManager()->GetSceneNavMeshData())
		return;
	if (!GetSceneManager()->GetSceneNavMeshData()->crowd)
		return;
	comp->NavAgent->AgentID = GetSceneManager()->GetSceneNavMeshData()->crowd->addAgent(posf, &comp->NavAgent->agentParams);
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
		agentcomp->NavAgent = std::make_shared<NavAgentData>();
		this->SetAcceleration(agentcomp, controller->Acceleration);
		this->SetRadius(agentcomp, controller->CapsuleControllerinfo.radius);
		this->SetSpeed(agentcomp, controller->MaxSpeed);
		SetHegiht(agentcomp, (controller->CapsuleControllerinfo.height + controller->CapsuleControllerinfo.radius));
		this->AddAgentToCrowd(agentcomp);
		return;

	}
	else
	{

		const auto& transformcomp = GetSceneManager()->GetComponent<TransformComponent>(gameObjectId);
		auto agentcomp = transformcomp->GetComponent<NavAgentComponent>();
		agentcomp->NavAgent = std::make_shared<NavAgentData>();
		this->SetAcceleration(agentcomp, 20 );
		this->SetRadius(agentcomp, 2);
		this->SetHegiht(agentcomp, 3);
		this->SetSpeed(agentcomp,10);
		this->AddAgentToCrowd(agentcomp);
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
	agentcomp->NavAgent.reset();
}

void NavAgentSystem::Finalize()
{
	COMPLOOP(NavAgentComponent, comp)
	{
		Finish(comp.GetEntityID());
	}
}

void NavAgentSystem::PhysicsLateUpdate(float deltaTime)
{
}

