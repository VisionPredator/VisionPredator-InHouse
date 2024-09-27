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
	// HP�� �ǰ� �޾��� ���� ������Ʈ�ϰ� �ʹ�.
	// -> �̺�Ʈ �Ŵ��� ���
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
	// else if ��ȣ�ۿ� ���� ��.
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
