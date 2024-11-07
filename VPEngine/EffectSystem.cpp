#include "pch.h"
#include "EffectSystem.h"

EffectSystem::EffectSystem(std::shared_ptr<SceneManager> sceneManager) : System(sceneManager)
{

}

void EffectSystem::BeginRenderUpdate(float deltaTime)
{
	COMPLOOP(EffectComponent, comp)
	{
		if (comp.GetHandle()->type().id() == Reflection::GetTypeID<EffectComponent>())
		{
			if (comp.HasComponent<MeshComponent>())
			{
				auto mesh = comp.GetComponent<MeshComponent>();

				mesh->FBX = comp.FBX;

				mesh->Renderdata->FBX = comp.FBX;
				mesh->Renderdata->textureName = comp.NoiseTexture;
				mesh->isEffect = comp.isEffect;
				mesh->Renderdata->punchEffect = comp.isEffect;
				mesh->Renderdata->duration = comp.Duration;
			}

			auto transform = comp.GetComponent<TransformComponent>();
			transform->Local_Location = comp.Offset;
			transform->Local_Scale = comp.Scale;
			transform->Local_Rotation = comp.Rotation;
		}
	}
}

void EffectSystem::RenderUpdate(float deltaTime)
{
	
}

void EffectSystem::LateRenderUpdate(float deltaTime)
{
}

void EffectSystem::EditorRenderUpdate(float deltaTime)
{
	BeginRenderUpdate(deltaTime);
}

void EffectSystem::ComponentAdded(Component* comp)
{
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<EffectComponent>())
	{
		auto effect = static_cast<EffectComponent*>(comp);

		if (comp->HasComponent<LifeTimeComponent>())
		{
			auto life = comp->GetComponent<LifeTimeComponent>();
			life->LifeTime = effect->Duration;
		}

		if (comp->HasComponent<MeshComponent>())
		{
			auto mesh = comp->GetComponent<MeshComponent>();

			mesh->FBX = effect->FBX;
			mesh->Renderdata->textureName = effect->NoiseTexture;
			mesh->isEffect = effect->isEffect;
		}

		auto transform = comp->GetComponent<TransformComponent>();
		transform->Local_Location = effect->Offset;
		transform->Local_Scale = effect->Scale;
	}
}

void EffectSystem::ComponentReleased(Component* comp)
{

}

void EffectSystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(EffectComponent, comp)
	{
		float preScale = comp.Duration;


		if (comp.Duration > 0)
		comp.Duration -= deltaTime;

		comp.Scale.x += deltaTime;
		comp.Scale.y += deltaTime;
		comp.Scale.z += deltaTime;

	}
}
