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
	// HP�� �ǰ� �޾��� ���� ������Ʈ�ϰ� �ʹ�.
	// -> �̺�Ʈ �Ŵ��� ���

	// hp �������� hp ��ġ �ؽ�Ʈ�� ����.
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

// OnChangeWeapon �̺�Ʈ�� �����..
void PlayerUISystem::UpdateWeaponUI(IdentityComponent& identityComp)
{
	/// ���� ������ ���� �� ------------------------------------------------------------------------
	
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

	// ��� �ִ� �ѱ��� �Ѿ� ���� ������Ʈ
	auto ammoStr = Util::ToWideChar(guncomp.CurrentBullet);
	ammoUIComp.Text = ammoStr;

	// ��� �ִ� ���� ������ ���� �ѱ� �̹��� ����
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
	// ���� ���൵�� ��ü �ð��� �����ɴϴ�.
	float progress = m_PlayerComp->TransformationProgress;
	float totalTime = m_PlayerComp->TransformationTime;

	// ��ü �ð��� 0 ������ ��� 0�� ��ȯ�Ͽ� ������ ������ �����մϴ�.
	if (totalTime <= 0.0f) return 0.0;

	// ���൵�� �ۼ�Ʈ�� ����մϴ�.
	float percentage = progress / totalTime;

	// ���� 1: 0% - 33%���� 0���� 2�� ���������� ����
	if (percentage <= 0.33f) {
		return (percentage / 0.33f) * 2.0;
	}
	// ���� 2: 33% - 66%���� 2�� ����
	else if (percentage <= 0.66f) {
		return 2.0;
	}
	// ���� 3: 66% - 100%���� 2���� 0���� ���������� ����
	else if (percentage <= 1.0f) {
		return 2.0 * (1.0f - (percentage - 0.66f) / 0.34f);
	}

	// ���൵�� 100%�� �ʰ��� ��� �⺻������ 0�� ��ȯ
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
