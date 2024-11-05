#include "pch.h"
#include "EntityRemoverSystem.h"
#include "VisPredComponents.h"

EntityRemoverSystem::EntityRemoverSystem(std::shared_ptr<SceneManager> scenemanager) :System(scenemanager)
{
}

void EntityRemoverSystem::EnterTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second)
{
	if (first->HasComponent<EntityRemoverComponet>())
		AddRemoveEntity(first, second);
	else if(second->HasComponent<EntityRemoverComponet>())
		AddRemoveEntity(second, first);
}

void EntityRemoverSystem::ExitTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second)
{
}

void EntityRemoverSystem::AddRemoveEntity(std::shared_ptr<Entity> remover, std::shared_ptr<Entity> removing)
{
	auto removecomp = remover->GetComponent<EntityRemoverComponet>();
	removecomp->RemoveEntitys.push_back(removing->GetEntityID());
}

void EntityRemoverSystem::Initialize()
{
}

void EntityRemoverSystem::Start(uint32_t gameObjectId)
{

}

void EntityRemoverSystem::Finish(uint32_t gameObjectId)
{
	auto entity = GetSceneManager()->GetEntity(gameObjectId);
	if (!entity->HasComponent<EntityRemoverComponet>())
		return;
	auto remover = entity->GetComponent<EntityRemoverComponet>();
	for (auto entityid : remover->RemoveEntitys)
	{
		if (!m_PhysicsEngine->IsStatic(entityid))
			GetSceneManager()->DestroyEntity(entityid);
	}
}
void EntityRemoverSystem::Finalize()
{
}

