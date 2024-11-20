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
			// button.Prefab_Pose는 std::tuple<VPMath::Vector3, VPMath::Vector3, VPMath::Vector3> 타입이라고 가정합니다.
			auto& prefabPose = button.Prefab_Pose;
			VPMath::Vector3 pos, rotation, scale;

			// 튜플의 값을 개별 변수로 분해합니다.
			std::tie(pos, rotation, scale) = prefabPose;
			// 분해된 값을 사용하여 함수 호출
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
			// button.Prefab_Pose는 std::tuple<VPMath::Vector3, VPMath::Vector3, VPMath::Vector3> 타입이라고 가정합니다.
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
