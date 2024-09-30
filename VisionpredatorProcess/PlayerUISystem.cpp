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
	// HP는 피격 받았을 때만 업데이트하고 싶다.
	// -> 이벤트 매니저 사용

	// hp 게이지와 hp 수치 텍스트를 변경.
	COMPLOOP(IdentityComponent, comp)
	{
		if (comp.UUID == "HPGageUI")
		{
			uint32_t id = comp.GetEntityID();
			auto ui = m_SceneManager.lock()->GetComponent<ImageComponent>(id);
			ui->RightPercent = 1.f - (playerComponent.HP * 0.01f);

			break;
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

			break;
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

	if (ui)
	{
		bool isInteracting = false;
		if (GetSceneManager()->HasEntity(playerComponent.SearchedItemID))
		{
			if (GetSceneManager()->GetEntity(playerComponent.SearchedItemID)->HasComponent<GunComponent>())
				isInteracting = true;
		}

		if (playerComponent.CurrentFSM == VisPred::Game::EFSM::ATTACK)
		{
			ui->TexturePath = "aim_attack.png";
		}
		else if (true == isInteracting)	// 상호작용 중일 때
		{
			ui->TexturePath = "aim_interact.png";
		}
		else    // IDLE
		{
			ui->TexturePath = "aim_base.png";
		}
	}
}

// OnChangeWeapon 이벤트를 만들까..
void PlayerUISystem::UpdateWeapon(const PlayerComponent& playerComponent)
{
	TextComponent* ammoUIComp = nullptr;
	ImageComponent* weaponUIComp = nullptr;

	// 총을 가지고 있을 때만 업데이트
	if (!playerComponent.HasGun)
	{
		// 총을 가지고 있지 않다면 총알 갯수는 0으로 설정.
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

	// 들고 있는 총기의 총알 갯수 업데이트
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

	// 들고 있는 무기 종류에 따른 총기 이미지 변경
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
