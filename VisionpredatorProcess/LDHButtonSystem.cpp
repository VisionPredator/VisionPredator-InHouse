#include "pch.h"
#include "LDHButtonSystem.h"
#include "AddPrefabButtonComponent.h"
#include "DestroySelfComponent.h"
#include "DeletePrefabComponent.h"
LDHButtonSystem::LDHButtonSystem(std::shared_ptr<SceneManager> scenemanager) :System(scenemanager)
{
}

void LDHButtonSystem::Update(float deltaTime)
{

	COMPLOOP(AddPrefabButtonComponent, button)
	{
		if (INPUTKEYDOWN(button.Button))
		{
			// button.Prefab_Pose�� std::tuple<VPMath::Vector3, VPMath::Vector3, VPMath::Vector3> Ÿ���̶�� �����մϴ�.
			auto& prefabPose = button.Prefab_Pose;
			VPMath::Vector3 pos, rotation, scale;

			// Ʃ���� ���� ���� ������ �����մϴ�.
			std::tie(pos, rotation, scale) = prefabPose;
			// ���ص� ���� ����Ͽ� �Լ� ȣ��
			GetSceneManager()->SpawnEditablePrefab(button.Prefabname, pos, rotation, scale);
		}
	}

	COMPLOOP(DestroySelfComponent, button)
		if (INPUTKEYDOWN(button.Button))
			button.GetEntity()->DestorySelf();

	COMPLOOP(DeletePrefabComponent, button)
	{
		if (INPUTKEYDOWN(button.DestoryOne))
		{
			// button.Prefab_Pose�� std::tuple<VPMath::Vector3, VPMath::Vector3, VPMath::Vector3> Ÿ���̶�� �����մϴ�.
			auto entity = GetSceneManager()->GetEntityByIdentityName(button.EntityIdentity);
			if (entity)
				entity->DestorySelf();
		}

	}
}

void LDHButtonSystem::BeginRenderUpdate(float deltaTime)
{
}

void LDHButtonSystem::RenderUpdate(float deltaTime)
{
}

void LDHButtonSystem::LateRenderUpdate(float deltaTime)
{
}

void LDHButtonSystem::EditorRenderUpdate(float deltaTime)
{
}
