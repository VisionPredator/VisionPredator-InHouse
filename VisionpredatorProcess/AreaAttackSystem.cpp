#include "pch.h"
#include "AreaAttackSystem.h"

AreaAttackSystem::AreaAttackSystem(std::shared_ptr<SceneManager> sceneManager) : System(sceneManager)
{
}


void AreaAttackSystem::EnterTrigger(std::pair<uint32_t, uint32_t> entitypair)
{
	auto entity_first = GetSceneManager()->GetEntity(entitypair.first);
	auto entity_second = GetSceneManager()->GetEntity(entitypair.second);

	//if (entity_first-> HasComponent<AreaAttackSystem>)
	//{
	//}
	//if (true)
	//{




	//}



}

void AreaAttackSystem::ExitTrigger(std::pair<uint32_t, uint32_t> entitypair)
{
}
