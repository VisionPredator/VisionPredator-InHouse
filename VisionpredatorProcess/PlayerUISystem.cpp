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
		UpdateFadeUI(comp);
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
	if (identityComp.UUID != "VPeyeUI" || !identityComp.HasComponent<ImageComponent>())
		return;
	auto& imagecomp = *identityComp.GetComponent<ImageComponent>();
	auto& VPUI = *identityComp.GetComponent<VPUIComponent>();
	VPMath::Color White{ 1,1,1,1 };
	VPMath::Color temp{};
	float percent = m_PlayerComp->VPGageProgress / m_PlayerComp->VPGageCoolTime;
	if (percent > 1)
		percent = 1;
	temp = VPMath::Color::Lerp(White, VPUI.ChangeColor, percent);
	if (percent>=1 || m_PlayerComp->ReadyToTransform)
	{
		imagecomp.TexturePath = VPUI.FullImage;
		imagecomp.Color = White;
		imagecomp.TopPercent = 0;

	}
	else
	{
		imagecomp.TopPercent = 1 - percent;

		imagecomp.TexturePath = VPUI.GageImage;
		imagecomp.Color = VPMath::Color::Lerp(White, VPUI.ChangeColor, percent);
	}

}
void PlayerUISystem::UpdateAim(IdentityComponent& identityComp)
{
	if (identityComp.UUID != "AimUI" || !identityComp.HasComponent<ImageComponent>())
		return;

	auto& imagecomp = *identityComp.GetComponent<ImageComponent>();
	auto& AimComp = *identityComp.GetComponent<AimUIComponent>();
	auto entity = GetSceneManager()->GetEntity(m_PlayerComp->SearchedItemID);
	imagecomp.Color.w = 1;
	if (!entity)
		imagecomp.Color.w = 0;
	else
	{
		if (entity->HasComponent<GunComponent>())
			imagecomp.TexturePath = AimComp.Interected;
		else if (entity->HasComponent<EnemyComponent>())
			imagecomp.TexturePath = AimComp.Aimed;
		else
			imagecomp.Color.w = 0;

	}
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
void PlayerUISystem::UpdateFadeUI(IdentityComponent& identityComp)
{
	if (identityComp.UUID != "FadeUI")
		return;
	auto& fadeui = *identityComp.GetComponent<ImageComponent>();
	fadeui.Color.w= TrasnformationFadePercent();
	TrasnformationFadePercent();


}
double PlayerUISystem::TrasnformationFadePercent() {
	// 현재 진행도와 전체 시간을 가져옵니다.
	float progress = m_PlayerComp->TransformationProgress;
	float totalTime = m_PlayerComp->TransformationTime;

	// 전체 시간이 0 이하일 경우 0을 반환하여 나누기 오류를 방지합니다.
	if (totalTime <= 0.0f) return 0.0;

	// 진행도를 퍼센트로 계산합니다.
	float percentage = progress / totalTime;

	// 구간 1: 0% - 33%에서 0에서 2로 선형적으로 증가
	if (percentage <= 0.33f) {
		return (percentage / 0.33f) * 2.0;
	}
	// 구간 2: 33% - 66%에서 2로 유지
	else if (percentage <= 0.66f) {
		return 2.0;
	}
	// 구간 3: 66% - 100%에서 2에서 0으로 선형적으로 감소
	else if (percentage <= 1.0f) {
		return 2.0 * (1.0f - (percentage - 0.66f) / 0.34f);
	}

	// 진행도가 100%를 초과할 경우 기본적으로 0을 반환
	return 0.0;
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
