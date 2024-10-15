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
	if (!m_PlayerComp)
		return;

	COMPLOOP(IdentityComponent, comp)
	{
		UpdateHP(comp);
		UpdateVPState(comp);
		UpdateAim(comp);
		UpdateWeaponUI(comp);
		UpdateInterectionUI(comp);
	}
}
void PlayerUISystem::Initialize()
{
	COMPLOOP(PlayerComponent, comp)
	{
		if (comp.GetComponent<IDComponent>()->Name == "Player")
		{
			m_PlayerComp = &comp;
			break;
		}
	}

}
void PlayerUISystem::Finalize()
{
	m_PlayerComp = nullptr;
}
void PlayerUISystem::UpdateHP(IdentityComponent& identityComp)
{
	// HP는 피격 받았을 때만 업데이트하고 싶다.
	// -> 이벤트 매니저 사용

	// hp 게이지와 hp 수치 텍스트를 변경.
	if (identityComp.UUID == "HPGageUI")
	{
		auto ui = identityComp.GetComponent<ImageComponent>();
		ui->RightPercent =1.f- static_cast<float>(m_PlayerComp->HP) / static_cast<float>(m_PlayerComp->MaxHP);

	}
	else if (identityComp.UUID == "HPCountUI")
	{
		auto ui = identityComp.GetComponent<TextComponent>();
		auto hpStr = Util::ToWideChar(m_PlayerComp->HP);
		ui->Text = hpStr;
	}
}
void PlayerUISystem::UpdateVPState(IdentityComponent& identityComp)
{

}
void PlayerUISystem::UpdateAim(IdentityComponent& identityComp)
{
	if (identityComp.UUID != "AimUI" || !identityComp.HasComponent<ImageComponent>())
		return;

	auto& imagecomp = *identityComp.GetComponent<ImageComponent>();
	if (GetSceneManager()->HasEntity(m_PlayerComp->SearchedItemID))
		if (GetSceneManager()->GetEntity(m_PlayerComp->SearchedItemID)->HasComponent<GunComponent>())
			imagecomp.TexturePath = "aim_interact.png";
		else
			imagecomp.TexturePath = "aim_base.png";

}

// OnChangeWeapon 이벤트를 만들까..
void PlayerUISystem::UpdateWeaponUI(IdentityComponent& identityComp)
{
	/// 총을 가지고 있을 때 ------------------------------------------------------------------------
	
	if (identityComp.UUID != "WeaponUI")
		return;

	auto& ammoUIComp = *identityComp.GetComponent<TextComponent>();
	auto& weaponUIComp = *identityComp.GetComponent<ImageComponent>();

	if (!m_PlayerComp->HasGun)
	{
		ammoUIComp.Text = L" ";
		weaponUIComp.TexturePath = "gun5.png";
		return;
	}


	auto& guncomp = *m_SceneManager.lock()->GetComponent<GunComponent>(m_PlayerComp->GunEntityID);

	// 들고 있는 총기의 총알 갯수 업데이트
	auto ammoStr = Util::ToWideChar(guncomp.CurrentBullet);
	ammoUIComp.Text = ammoStr;

	// 들고 있는 무기 종류에 따른 총기 이미지 변경
	switch (guncomp.Type)
	{
	case VisPred::Game::GunType::PISTOL:
	{
		weaponUIComp.TexturePath = "gun3.png";
		break;
	}
	case VisPred::Game::GunType::RIFLE:
	{
		weaponUIComp.TexturePath = "gun1.png";
		break;
	}
	case VisPred::Game::GunType::SHOTGUN:
	{
		weaponUIComp.TexturePath = "gun4.png";
		break;
	}
	default:
		break;
	}
}

void PlayerUISystem::UpdateInterectionUI(IdentityComponent& identityComp)
{
	if (identityComp.UUID != "InterectionUI")
		return;
	if (!identityComp.HasComponent<TextComponent>() || !identityComp.HasComponent<TextComponent>())
		return;
	if (!GetSceneManager()->HasEntity(m_PlayerComp->SearchedItemID))
	{
		InterectUIReset(identityComp);
		return;
	}
	InterectUIReset(identityComp);
	auto entity = GetSceneManager()->GetEntity(m_PlayerComp->SearchedItemID).get();
	if (InterectingGun(identityComp, entity)) {}
	else if (InterectingCloset(identityComp, entity)) {}
	else if (InterectingDoor(identityComp, entity)) {}
}
bool PlayerUISystem::InterectUIReset(IdentityComponent& identitycomp)
{
	auto& textUI = *identitycomp.GetComponent<TextComponent>();
	auto& imageUI = *identitycomp.GetComponent<ImageComponent>();
	textUI.Color.w = 0;
	imageUI.Color.w = 0;
	return true;
}
bool PlayerUISystem::InterectingGun(IdentityComponent& identitycomp, Entity* selectedentity)
{
	if (!selectedentity->HasComponent<GunComponent>())
		return false;

	auto guncomp = selectedentity->GetComponent<GunComponent>();
	auto& textUI = *identitycomp.GetComponent<TextComponent>();
	auto& imageUI = *identitycomp.GetComponent<ImageComponent>();

	textUI.Color.w = 1;
	imageUI.Color.w = 0.3;

	switch (guncomp->Type)
	{
	case VisPred::Game::GunType::PISTOL:
	{
		textUI.Text = L"Pistol";
		break;
	}
	case VisPred::Game::GunType::SHOTGUN:
	{
		textUI.Text = L"Shotgun";

		break;
	}
	case VisPred::Game::GunType::RIFLE:
	{
		textUI.Text = L"Assault rifle";
		break;
	}
	default:
		break;
	}
}
bool PlayerUISystem::InterectingDoor(IdentityComponent& playerComponent, Entity* selectedentity)
{
	return false;
}
bool PlayerUISystem::InterectingCloset(IdentityComponent& playerComponent, Entity* selectedentity)
{
	return false;
}

void PlayerUISystem::OnGunShoot(std::any data)
{
}
