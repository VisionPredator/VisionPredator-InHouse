#include "pch.h"
#include "PlayerUISystem.h"
#include "../VPGraphics/Util.h"

PlayerUISystem::PlayerUISystem(const std::shared_ptr<SceneManager>& sceneManager)
	: System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnGunShoot", CreateSubscriber(&PlayerUISystem::OnGunShoot));
}

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

	// hp �������� hp ��ġ �ؽ�Ʈ�� ����.
	COMPLOOP(IdentityComponent, comp)
	{
		if (comp.UUID == "HPGageUI")
		{
			uint32_t id = comp.GetEntityID();
			auto ui = m_SceneManager.lock()->GetComponent<ImageComponent>(id);
			ui->RightPercent = 1.f - (playerComponent.HP * 0.01f);
		}
	}

	COMPLOOP(IdentityComponent, comp)
	{
		if (comp.UUID == "HPCountUI")
		{
			uint32_t id = comp.GetEntityID();
			auto ui = m_SceneManager.lock()->GetComponent<TextComponent>(id);
			auto hpStr = Util::ToWideChar(playerComponent.HP);
			ui->Text = hpStr;
		}
	}
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
				auto ui = m_SceneManager.lock()->GetComponent<ImageComponent>(id);
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
				auto ui = m_SceneManager.lock()->GetComponent<ImageComponent>(id);
				ui->TexturePath = "aim_base.png";
				break;
			}
		}
	}
}

// OnChangeWeapon �̺�Ʈ�� �����..
void PlayerUISystem::UpdateWeapon(const PlayerComponent& playerComponent)
{
	// ���� ������ ���� ���� ������Ʈ
	if (!playerComponent.HasGun)
	{
		// ���� ������ ���� �ʴٸ� �Ѿ� ������ 0���� ����.
		COMPLOOP(IdentityComponent, comp)
		{
			if (comp.UUID == "AmmoUI")
			{
				uint32_t id = comp.GetEntityID();
				if (false == m_SceneManager.lock()->HasComponent<TextComponent>(id))
					continue;

				auto ui = m_SceneManager.lock()->GetComponent<TextComponent>(id);
				ui->Text = L"0";
				break;
			}
		}
		return;
	}

	COMPLOOP(GunComponent, comp)
	{
		// �÷��̾ ������ ���̶��
		if (comp.GetEntityID() == playerComponent.GunEntityID)
		{
			COMPLOOP(IdentityComponent, uiComp)
			{
				if (uiComp.UUID == "AmmoUI")
				{
					uint32_t id = uiComp.GetEntityID();
					if (false == m_SceneManager.lock()->HasComponent<TextComponent>(id))
						continue;

					auto ui = m_SceneManager.lock()->GetComponent<TextComponent>(id);
					auto ammoStr = Util::ToWideChar(comp.CurrentBullet);
					ui->Text = ammoStr;
				}
			}
		}
	}

}

void PlayerUISystem::OnGunShoot(std::any data)
{

}
