#include "pch.h"
#include "PingSystem.h"
#include "EventManager.h"
#include "PingAccessComponent.h"
#include "PingComponent.h"
#include "PingAccessResetComponent.h"
PingSystem::PingSystem(std::shared_ptr<SceneManager> sceneManager) :System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnInterected", CreateSubscriber(&PingSystem::OnInterected));
}

void PingSystem::OnInterected(std::any interective_interector)
{
	auto [interected, interector] = std::any_cast<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>>(interective_interector);

	auto ping = GetSceneManager()->GetChildEntityComp_HasComp<PingComponent>(interected->GetEntityID());
	auto pingaccess = GetSceneManager()->GetChildEntityComp_HasComp<PingAccessComponent>(interected->GetEntityID());
	auto pingreset = GetSceneManager()->GetChildEntityComp_HasComp<PingAccessResetComponent>(interected->GetEntityID());

	if (ping)
	{
		auto pingcomp = ping->GetComponent<PingComponent>();
		pingcomp->IsOn = false;
	}

	if (pingaccess)
	{

		if (!pingaccess->IsUsed)
		{
			if (!pingaccess->PingIdentity.empty())
				for (auto& identity : pingaccess->PingIdentity)
				{
					auto entity = GetSceneManager()->GetEntityByIdentityName(identity);
					if (entity && entity->HasComponent<PingComponent>())
					{
						entity->GetComponent<PingComponent>()->IsOn = true;
					}
				}
			pingaccess->IsUsed = true;
		}
	}

	if (pingreset)
	{
		if (!pingreset->PingIdentity.empty())
			for (auto& identity : pingreset->PingIdentity)
			{
				auto entity = GetSceneManager()->GetEntityByIdentityName(identity);
				if (entity && entity->HasComponent<PingAccessComponent>())
				{
					entity->GetComponent<PingAccessComponent>()->IsUsed = false;
				}
			}
	}
}



void PingSystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(PingComponent, pingcomp)
	{
		if (pingcomp.IsOn && pingcomp.HasComponent<ImageComponent>())
		{
			pingcomp.GetComponent<ImageComponent>()->Color.w = 1;
		}
		else
		{
			pingcomp.GetComponent<ImageComponent>()->Color.w = 0;

		}
	}
}
