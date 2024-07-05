#include "pch.h"
#include "LightSystem.h"

LightSystem::LightSystem(SceneManager* sceneManager)
	: System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnAddedComponent", CreateSubscriber(&LightSystem::OnAddedComponent));
	EventManager::GetInstance().Subscribe("OnReleasedComponent", CreateSubscriber(&LightSystem::OnReleasedComponent));
}

void LightSystem::OnAddedComponent(std::any data)
{
	auto comp = std::any_cast<Component*>(data);
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<LightComponent>())
	{
		LightComponent* lightComp = static_cast<LightComponent*>(comp);
		LightData temp;
		temp.attenuation= lightComp->attenuation;
		temp.color = { lightComp->color .x,lightComp->color .y,lightComp->color .z};
		temp.direction = lightComp->direction;
		temp.intensity = lightComp->intensity;
		temp.pos = lightComp->GetComponent<TransformComponent>()->World_Location;
		temp.range= lightComp->range;
		temp.spot= lightComp->spot;
		m_Graphics->AddLight(lightComp->GetEntityID(), lightComp->type, temp);
		return;
	}
}

void LightSystem::OnReleasedComponent(std::any data)
{
	auto comp = std::any_cast<Component*>(data);
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<LightComponent>())
	{
		LightComponent* lightComp = static_cast<LightComponent*>(comp);
		m_Graphics->EraseLight(lightComp->GetEntityID(), lightComp->type);

		return;
	}
}

void LightSystem::Update(float deltaTime)
{
	for (LightComponent& lightComp : COMPITER(LightComponent))
	{
		lightComp.direction.Normalize();
	}
}

void LightSystem::FixedUpdate(float deltaTime)
{

}

void LightSystem::RenderUpdate(float deltaTime)
{
	for (LightComponent& lightComp : COMPITER(LightComponent))
	{
		LightData temp;
		temp.attenuation = lightComp.attenuation;
		temp.color = { lightComp.color.x,lightComp.color.y,lightComp.color.z };
		temp.direction = lightComp.direction;
		temp.intensity = lightComp.intensity;
		temp.pos = lightComp.GetComponent<TransformComponent>()->World_Location;
		temp.range = lightComp.range;

		temp.spot = lightComp.spot;
		temp.type = static_cast<float>(lightComp.type);
		m_Graphics->UpdateLightData(lightComp.GetEntityID(), lightComp.type, temp);

		debug::SphereInfo sphereInfo;
		sphereInfo.Sphere.Center = temp.pos;
		sphereInfo.Sphere.Radius = temp.range;
		sphereInfo.Color = DirectX::SimpleMath::Color(temp.color);


		debug::RayInfo ray;
		ray.Color = DirectX::SimpleMath::Color(temp.color);
		ray.Direction = { temp.direction.x * temp.range, temp.direction.y * temp.range,temp.direction.z * temp.range };
		ray.Origin = temp.pos;
		ray.Normalize = false;



		m_Graphics->DrawSphere(sphereInfo);
		m_Graphics->DrawRay(ray);

	}
}
