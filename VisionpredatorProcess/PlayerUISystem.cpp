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

			continue;
		}

		if (comp.UUID == "HPCountUI")
		{
			uint32_t id = comp.GetEntityID();
			auto ui = m_SceneManager.lock()->GetComponent<TextComponent>(id);
			auto hpStr = Util::ToWideChar(playerComponent.HP);
			ui->Text = hpStr;
			continue;
		}
	}
}

void PlayerUISystem::UpdateVPState(const PlayerComponent& playerComponent)
{

}

void PlayerUISystem::UpdateAim(const PlayerComponent& playerComponent)
{
	ImageComponent* ui = nullptr;

	COMPLOOP(IdentityComponent, identity)
	{
		if (identity.UUID == "AimUI")
		{
			uint32_t id = identity.GetEntityID();
			if (false == m_SceneManager.lock()->HasComponent<ImageComponent>(id))
				return;
			ui = m_SceneManager.lock()->GetComponent<ImageComponent>(id);

			break;
		}
	}

	bool isInteracting = false;
	if (GetSceneManager()->HasEntity(playerComponent.SearchedItemID))
	{
		if (GetSceneManager()->GetEntity(playerComponent.SearchedItemID)->HasComponent<GunComponent>())
			isInteracting = true;
	}

	if (ui != nullptr)
	{
		if (isInteracting)
		{
			ui->TexturePath = "aim_interact.png";
		}
		else    // IDLE
		{
			ui->TexturePath = "aim_base.png";
		}
	}
}

// OnChangeWeapon �̺�Ʈ�� �����..
void PlayerUISystem::UpdateWeapon(const PlayerComponent& playerComponent)
{
	/// ���� ������ ���� ���� �� ------------------------------------------------------------------------
	// ��ȣ�ۿ� �� ���� ���� UI ���
	bool isInteracting = false;
	VisPred::Game::GunType gunTypeNoInteract;
	if (GetSceneManager()->HasEntity(playerComponent.SearchedItemID))
	{
		if (GetSceneManager()->GetEntity(playerComponent.SearchedItemID)->HasComponent<GunComponent>())
		{
			isInteracting = true;

			auto gunComp = m_SceneManager.lock()->GetComponent<GunComponent>(playerComponent.SearchedItemID);
			gunTypeNoInteract = gunComp->Type;
		}
	}

	TextComponent* textUI = nullptr;
	ImageComponent* imageUI = nullptr;

	int roopCount = 0;
	COMPLOOP(IdentityComponent, comp)
	{
		if (roopCount == 2)
			break;

		if (comp.UUID == "WeaponInfoTextUI")
		{
			uint32_t id = comp.GetEntityID();

			if (false == m_SceneManager.lock()->HasComponent<TextComponent>(id))
				continue;

			textUI = m_SceneManager.lock()->GetComponent<TextComponent>(id);
			roopCount++;
		}
		else if (comp.UUID == "WeaponInfoBackgroundUI")
		{
			uint32_t id = comp.GetEntityID();

			if (false == m_SceneManager.lock()->HasComponent<ImageComponent>(id))
				continue;

			imageUI = m_SceneManager.lock()->GetComponent<ImageComponent>(id);
			roopCount++;
		}
	}

	if (true == isInteracting)
	{
		if (textUI != nullptr)
		{
			textUI->Color.w = 1;

			switch (gunTypeNoInteract)
			{
				case VisPred::Game::GunType::PISTOL:
				{
					textUI->Text = L"Pistol";
					break;
				}
				case VisPred::Game::GunType::SHOTGUN:
				{
					textUI->Text = L"Shotgun";

					break;
				}
				case VisPred::Game::GunType::RIFLE:
				{
					textUI->Text = L"Assault rifle";
					break;
				}
				default:
					break;
			}
		}

		if (imageUI != nullptr)
		{
			imageUI->Color.w = 0.3;
		}

	}
	else
	{
		if (textUI != nullptr)
		{
			textUI->Color.w = 0;
		}
		if (imageUI != nullptr)
		{
			imageUI->Color.w = 0;
		}
	}

	/// ���� ������ ���� �� ------------------------------------------------------------------------
	TextComponent* ammoUIComp = nullptr;
	ImageComponent* weaponUIComp = nullptr;

	if (!playerComponent.HasGun)
	{
		// ���� ������ ���� �ʴٸ� �Ѿ� ������ 0���� ����.
		COMPLOOP(IdentityComponent, comp)
		{
			if (comp.UUID == "AmmoUI")
			{
				uint32_t id = comp.GetEntityID();
				if (false == m_SceneManager.lock()->HasComponent<TextComponent>(id))
					break;

				ammoUIComp = m_SceneManager.lock()->GetComponent<TextComponent>(id);
				ammoUIComp->Text = L"0";

				break;
			}
		}

		COMPLOOP(IdentityComponent, comp)
		{
			if (comp.UUID == "WeaponUI")
			{
				uint32_t id = comp.GetEntityID();
				if (false == m_SceneManager.lock()->HasComponent<ImageComponent>(id))
					break;

				weaponUIComp = m_SceneManager.lock()->GetComponent<ImageComponent>(id);
				weaponUIComp->TexturePath = "gun5.png";
			}
		}

		return;
	}

	uint32_t gunID = playerComponent.GunEntityID;
	if (false == m_SceneManager.lock()->HasComponent<GunComponent>(gunID))
		return;

	auto gun = m_SceneManager.lock()->GetComponent<GunComponent>(gunID);

	COMPLOOP(IdentityComponent, uiComp)
	{
		if (uiComp.UUID == "AmmoUI")
		{
			uint32_t uiID = uiComp.GetEntityID();

			if (false == m_SceneManager.lock()->HasComponent<TextComponent>(uiID))
				break;

			ammoUIComp = m_SceneManager.lock()->GetComponent<TextComponent>(uiID);
			break;
		}
	}

	// ��� �ִ� �ѱ��� �Ѿ� ���� ������Ʈ
	if (ammoUIComp != nullptr)
	{
		auto ammoStr = Util::ToWideChar(gun->CurrentBullet);
		ammoUIComp->Text = ammoStr;
	}

	COMPLOOP(IdentityComponent, comp)
	{
		if (comp.UUID == "WeaponUI")
		{
			uint32_t id = comp.GetEntityID();
			if (false == m_SceneManager.lock()->HasComponent<ImageComponent>(id))
				break;

			weaponUIComp = m_SceneManager.lock()->GetComponent<ImageComponent>(id);
			break;
		}
	}

	VisPred::Game::GunType gunType;
	if (m_SceneManager.lock()->HasComponent<GunComponent>(playerComponent.GunEntityID))
	{
		auto gunComp = m_SceneManager.lock()->GetComponent<GunComponent>(playerComponent.GunEntityID);
		gunType = gunComp->Type;
	}

	// ��� �ִ� ���� ������ ���� �ѱ� �̹��� ����
	if (weaponUIComp != nullptr)
	{
		switch (gunType)
		{
			case VisPred::Game::GunType::PISTOL:
			{
				weaponUIComp->TexturePath = "gun3.png";
				break;
			}
			case VisPred::Game::GunType::RIFLE:
			{
				weaponUIComp->TexturePath = "gun1.png";
				break;
			}
			case VisPred::Game::GunType::SHOTGUN:
			{
				weaponUIComp->TexturePath = "gun4.png";
				break;
			}
			default:
				break;
		}
	}

	
}

void PlayerUISystem::OnGunShoot(std::any data)
{

}
