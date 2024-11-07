#include "pch.h"
#include "EffectSystem.h"

void EffectSystem::BeginRenderUpdate(float deltaTime)
{
}

void EffectSystem::RenderUpdate(float deltaTime)
{
	COMPLOOP(EffectComponent, comp)
	{
		if (comp.GetHandle()->type().id() == Reflection::GetTypeID<EffectComponent>())
		{
			if (comp.HasComponent<MeshComponent>())
			{
				auto mesh = comp.GetComponent<MeshComponent>();

				mesh->Renderdata->FBX = comp.FBX;
				mesh->Renderdata->textureName = comp.NoiseTexture;
				mesh->Renderdata->punchEffect = comp.isEffect;
			}

			auto transform = comp.GetComponent<TransformComponent>();
			transform->Local_Location = comp.Offset;
			transform->Local_Scale = comp.Scale;
			transform->Local_Rotation = comp.Rotation;
		}
	}
}

void EffectSystem::LateRenderUpdate(float deltaTime)
{
}

void EffectSystem::EditorRenderUpdate(float deltaTime)
{
}

void EffectSystem::ComponentAdded(Component* comp)
{
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<EffectComponent>())
	{
		auto effect = static_cast<EffectComponent*>(comp);

		if (comp->HasComponent<MeshComponent>())
		{
			auto mesh = comp->GetComponent<MeshComponent>();

			mesh->FBX = effect->FBX;
			mesh->Renderdata->textureName = effect->NoiseTexture;
		}

		auto transform = comp->GetComponent<TransformComponent>();
		transform->Local_Location = effect->Offset;
		transform->Local_Scale = effect->Scale;
	}
}

void EffectSystem::ComponentReleased(Component* comp)
{

}
