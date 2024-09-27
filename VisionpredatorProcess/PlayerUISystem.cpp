#include "pch.h"
#include "PlayerUISystem.h"

void PlayerUISystem::Update(float deltaTime)
{
	COMPLOOP(PlayerComponent, comp)
	{
		UpdateHP(comp);
		UpdateVPState(comp);
		UpdateAim(comp);
		UpdateWeapon(comp);
	}
}

void PlayerUISystem::UpdateHP(const PlayerComponent& playerComponent)
{
	// HP는 피격 받았을 때만 업데이트하고 싶다.
	// -> 이벤트 매니저 사용
}

void PlayerUISystem::UpdateVPState(const PlayerComponent& playerComponent)
{

}

void PlayerUISystem::UpdateAim(const PlayerComponent& playerComponent)
{
	if (playerComponent.CurrentFSM == VisPred::Game::EFSM::ATTACK)
	{
		COMPLOOP(IdentityComponent, comp)
		{
			if (comp.UUID == "AimUI")
			{
				uint32_t id = comp.GetEntityID();
				auto ui = SceneManager::GetComponent<ImageComponent>(id);
				ui->TexturePath = "aim_attack.png";
				break;
			}
		}
	}
	// else if 상호작용 중일 때.
	else    // IDLE
	{
		COMPLOOP(IdentityComponent, comp)
		{
			if (comp.UUID == "AimUI")
			{
				uint32_t id = comp.GetEntityID();
				auto ui = SceneManager::GetComponent<ImageComponent>(id);
				ui->TexturePath = "aim_base.png";
				break;
			}
		}
	}
}

void PlayerUISystem::UpdateWeapon(const PlayerComponent& playerComponent)
{

}
