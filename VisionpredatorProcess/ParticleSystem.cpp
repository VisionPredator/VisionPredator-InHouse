#include "pch.h"
#include "ParticleSystem.h"
#include "ParticleOwnerComponent.h"
#include "FollowComponent.h"
#include "EventManager.h"
ParticleSystem::ParticleSystem(std::shared_ptr<SceneManager> scenemanager):System(scenemanager)
{
	EventManager::GetInstance().Subscribe("OnFollowParticle", CreateSubscriber(&ParticleSystem::OnFollowParticle));
}

void ParticleSystem::OnFollowParticle(std::any data)
{
    // data에서 엔티티 ID를 추출하고 해당 엔티티를 가져옴
    auto entityID = std::any_cast<uint32_t>(data);
    auto entity = GetSceneManager()->GetEntity(entityID);
    if (!entity || !entity->HasComponent<ParticleOwnerComponent>())
        return;

    // ParticleOwnerComponent를 가져오고 관련 파티클이 있는지 확인
    auto smokecomp = entity->GetComponent<ParticleOwnerComponent>();
    if (smokecomp->ParticleName.empty())
        return;
        
    COMPLOOP(ParticleComponent, particle)
    {

		auto name = particle.GetComponent<IDComponent>()->Name;
        if (name != smokecomp->ParticleName)
            continue;

        if (particle.IsPlaying)
        {
            if (particle.HasComponent<FollowComponent>())
            {
                auto followComp = particle.GetComponent<FollowComponent>();
                if (followComp->FollowingEntityID == entityID)
                {
                    particle.Restart = true;
                    break;
                }
            }
            continue;
        }

        // 파티클을 시작하고 엔티티를 따라가도록 설정
        particle.Restart = true;
        particle.IsPlaying = true;

        if (particle.HasComponent<FollowComponent>())
        {
            auto followComp = particle.GetComponent<FollowComponent>();
            followComp->FollowingEntityID = entityID;
            followComp->Isfollowing = true;
        }
        break;
    }
}

void ParticleSystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(FollowComponent, comp)
	{
		if (!comp.Isfollowing)
			continue;
		auto entity =GetSceneManager()->GetEntity(comp.FollowingEntityID);
        if (!entity)
        {
            comp.Isfollowing = false;
			continue;
                
        }
		auto transform = entity->GetComponent<TransformComponent>();
		auto followtrans = comp.GetComponent<TransformComponent>();
		followtrans->SetWorldLocation(transform->World_Location);
		followtrans->SetWorldRotation(transform->World_Rotation);
	}
}
