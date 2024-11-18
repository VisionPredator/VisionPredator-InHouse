#include "pch.h"
#include "VPDetectionSystem.h"
#include "VPDetectionComponent.h"
#include "EnemyComponent.h"
#include "InterectiveComponent.h"
#include "GunComponent.h"

VPDetectionSystem::VPDetectionSystem(std::shared_ptr<SceneManager> scenemanger):System(scenemanger)
{


}


void VPDetectionSystem::EntityEnter(std::shared_ptr<Entity> detected, std::shared_ptr<Entity> detector)
{
	auto detection = detector->GetComponent<VPDetectionComponent>();
	if (detected->HasComponent<EnemyComponent>())
	{
		detection->EnteredEnemy.push_back(detected->GetEntityID());


		if (!detection->IsDectionMode)
			return;
		auto meshvector = GetSceneManager()->GetChildEntityComps_HasComp<MeshComponent>(detected->GetEntityID());
		if (!meshvector.empty())
				for (auto mesh : meshvector)
					mesh->InteractiveColor = detection->EnemyColor;
	}
	else if (detected->HasComponent<InterectiveComponent>() && !detected->HasComponent<GunComponent>())
	{
		detection->InterecableEntity.push_back(detected->GetEntityID());

		if (!detection->IsDectionMode)
			return;
		auto meshvector = GetSceneManager()->GetChildEntityComps_HasComp<MeshComponent>(detected->GetEntityID());
		if (!meshvector.empty())
			for (auto mesh : meshvector)
				mesh->InteractiveColor = detection->InterectColor;
	}
}

void VPDetectionSystem::EntityExit(std::shared_ptr<Entity> detected, std::shared_ptr<Entity> detector) 
{
	auto detection = detector->GetComponent<VPDetectionComponent>();
	uint32_t entityID = detected->GetEntityID();

	if (detected->HasComponent<EnemyComponent>()) 
		detection->EnteredEnemy.remove(entityID);
	if (detected->HasComponent<InterectiveComponent>()&& !detected->HasComponent<GunComponent>())
		detection->InterecableEntity.remove(entityID);

	auto meshvector = GetSceneManager()->GetChildEntityComps_HasComp<MeshComponent>(entityID);
	if (!meshvector.empty())
			for (auto mesh : meshvector)
				mesh->InteractiveColor = {};
}

void VPDetectionSystem::EnterTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second)
{
	if (second->HasComponent<VPDetectionComponent>())
		EntityEnter(first, second);
	else if (first->HasComponent<VPDetectionComponent>())
		EntityEnter(second, first);
}

void VPDetectionSystem::ExitTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second)
{
	if (first->HasComponent<VPDetectionComponent>())
		EntityEnter(first, second);
	else if (second->HasComponent<VPDetectionComponent>())
		EntityEnter(second, first);
}

void VPDetectionSystem::Initialize()
{
}

void VPDetectionSystem::Start(uint32_t gameObjectId)
{
}

void VPDetectionSystem::Finish(uint32_t gameObjectId)
{
}

void VPDetectionSystem::Finalize()
{
}

void VPDetectionSystem::OnInterectionChanged(std::any data)
{
	auto entityId = std::any_cast<uint32_t> (data);

	auto interection = GetSceneManager()->GetComponent<InterectiveComponent>(entityId);


}

void VPDetectionSystem::OnDetectiveMode(std::any entityID)
{
	auto entityid = std::any_cast<uint32_t>(entityID);

	// VPDetectionComponent�� ������ ��ȯ
	if (!GetSceneManager()->HasComponent<VPDetectionComponent>(entityid))
		return;

	auto detection = GetSceneManager()->GetComponent<VPDetectionComponent>(entityid);

	if (detection->IsDectionMode)
	{
		for (auto it = detection->InterecableEntity.begin(); it != detection->InterecableEntity.end(); ) {
			uint32_t enemyid = *it;
			auto entity = GetSceneManager()->GetEntity(enemyid);
			// ��ƼƼ�� �������� ������ ����Ʈ���� ���� �� �߰� �۾� ����
			if (!entity)
				it = detection->InterecableEntity.erase(it); // ���� �� iterator ������Ʈ
			else
			{
				if (entity->HasComponent<InterectiveComponent>() && entity->GetComponent<InterectiveComponent>()->IsInterective  && !entity->HasComponent<GunComponent>() )
				{
					auto meshvector = GetSceneManager()->GetChildEntityComps_HasComp<MeshComponent>(entity->GetEntityID());
					if (!meshvector.empty())
						for (auto mesh : meshvector)
							mesh->InteractiveColor = detection->InterectColor;
				}

				++it; // ��ƼƼ�� �����ϸ� ���� iterator�� �̵�
			}
		}
	}
	else
	{
		for (auto it = detection->InterecableEntity.begin(); it != detection->InterecableEntity.end(); ) {
			uint32_t enemyid = *it;
			auto entity = GetSceneManager()->GetEntity(enemyid);
			// ��ƼƼ�� �������� ������ ����Ʈ���� ���� �� �߰� �۾� ����
			if (!entity)
				it = detection->InterecableEntity.erase(it); // ���� �� iterator ������Ʈ
			else
			{
				if (entity->HasComponent<InterectiveComponent>() && !entity->HasComponent<GunComponent>())
				{
					auto meshvector = GetSceneManager()->GetChildEntityComps_HasComp<MeshComponent>(entity->GetEntityID());
					if (!meshvector.empty())
						for (auto mesh : meshvector)
							mesh->InteractiveColor = {};
				}

				++it; // ��ƼƼ�� �����ϸ� ���� iterator�� �̵�
			}
		}
	}






}
