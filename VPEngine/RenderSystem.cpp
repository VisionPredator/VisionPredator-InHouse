#include "pch.h"
#include "RenderSystem.h"
#include "Components.h"
#include "EventManager.h"


RenderSystem::RenderSystem(std::shared_ptr<SceneManager> sceneManager)
	: System(sceneManager)
{
}
void RenderSystem::ComponentAdded(Component* comp)
{
	// Particle Object
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<ParticleComponent>())
	{
		ParticleComponent* component = static_cast<ParticleComponent*>(comp);
		std::string Path;
		Path.assign(component->TexturePath.begin(), component->TexturePath.end());

		effect::ParticleInfo info;
		info.TexturePath = Path;
		m_Graphics->CreateParticleObject(component->GetEntityID(), info);
		return;
	}
}

void RenderSystem::ComponentReleased(Component* comp)
{
	// particle object
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<ParticleComponent>())
	{
		ParticleComponent* component = static_cast<ParticleComponent*>(comp);
		m_Graphics->DeleteParticleObjectByID(component->GetEntityID());
	}


}



void RenderSystem::BeginRenderUpdate(float deltaTime)
{
	///렌더데이터 업데이트 정보 업데이트 하기.
	for (ParticleComponent& component : COMPITER(ParticleComponent))
	{
		effect::ParticleInfo info;
		info.TexturePath = component.TexturePath;
		info.MaxParticles = component.MaxParticle;

		m_Graphics->UpdateParticleObject(component.GetComponent<IDComponent>()->GetEntityID(), info);
	}
	m_Graphics->CulingUpdate();
	m_Graphics->AnimationUpdate(deltaTime);
	m_Graphics->Update(deltaTime);
	m_Graphics->EndUpdate(deltaTime);
}

void RenderSystem::RenderUpdate(float deltaTime)
{
	///변경된 RenderData 를 통해서 컬링하기.


	///이후 소켓에 의한 렌더 데이터 수정하기.
	///Socek
	/// Mesh
	/// 
	/// 
	/// 
}

void RenderSystem::LateRenderUpdate(float deltaTime)
{
	///소켓이 있는 애들만 RenderData 수정하기.

}

void RenderSystem::EditorRenderUpdate(float deltaTime)
{
	BeginRenderUpdate(deltaTime);
}








