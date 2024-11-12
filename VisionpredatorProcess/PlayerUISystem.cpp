#include "pch.h"
#include "PlayerUISystem.h"
#include "../VPGraphics/Util.h"
#include "VisPredComponents.h"
PlayerUISystem::PlayerUISystem(const std::shared_ptr<SceneManager>& sceneManager)
	: System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnUpdateSearchUI", CreateSubscriber(&PlayerUISystem::OnUpdateSearchUI));
	EventManager::GetInstance().Subscribe("OnUpdateWeaponUI", CreateSubscriber(&PlayerUISystem::OnUpdateWeaponUI));
	EventManager::GetInstance().Subscribe("OnUpdateHP", CreateSubscriber(&PlayerUISystem::OnUpdateHP));
	EventManager::GetInstance().Subscribe("OnResetInterectionUI", CreateSubscriber(&PlayerUISystem::OnResetInterectionUI));
	EventManager::GetInstance().Subscribe("OnDamaged", CreateSubscriber(&PlayerUISystem::OnDamaged));
	EventManager::GetInstance().Subscribe("OnShoot", CreateSubscriber(&PlayerUISystem::OnShoot));
	EventManager::GetInstance().Subscribe("OnUpdateVPState", CreateSubscriber(&PlayerUISystem::OnUpdateVPState));
	EventManager::GetInstance().Subscribe("OnHideBullet", CreateSubscriber(&PlayerUISystem::OnHideBullet));
}

void PlayerUISystem::Update(float deltaTime)
{
	if (!m_PlayerUI.lock())
		return;
	if (!m_PlayerUI.lock()->GetComponent<PlayerUIComponent>()->PlayerEntity.lock())
		return;
	//UpdateVPState();
	UpdateFadeUI();
	UpdateHitUI(deltaTime);
	UpdateAim();
	//UpdateWeaponUI();
}
void PlayerUISystem::Initialize()
{
	COMPLOOP(PlayerUIComponent, comp)
	{
		Start(comp.GetEntityID());
	}
}
void PlayerUISystem::Start(uint32_t gameObjectId)
{
	if (m_PlayerUI.lock())
		return;
	auto entity = GetSceneManager()->GetEntity(gameObjectId);
	if (entity->HasComponent<PlayerUIComponent>())
	{
		m_PlayerUI = entity;
		auto playerUI = entity->GetComponent<PlayerUIComponent>();
		playerUI->PlayerEntity = GetSceneManager()->GetEntityByIdentityName(playerUI->Player);
		playerUI->PlayerEntity = GetSceneManager()->GetEntityByIdentityName(playerUI->Player);
		playerUI->FadeEntity = GetSceneManager()->GetEntityByIdentityName(playerUI->FadeUI);
		playerUI->HPGageEntity = GetSceneManager()->GetEntityByIdentityName(playerUI->HPGage);
		playerUI->HPBackEntity = GetSceneManager()->GetEntityByIdentityName(playerUI->HPBackGround);
		playerUI->HPBoarderEntity = GetSceneManager()->GetEntityByIdentityName(playerUI->HPBoarder);
		playerUI->AimEntity = GetSceneManager()->GetEntityByIdentityName(playerUI->AimUI);
		playerUI->VPeyeEntity = GetSceneManager()->GetEntityByIdentityName(playerUI->VPeyeUI);
		playerUI->WeaponEntity = GetSceneManager()->GetEntityByIdentityName(playerUI->WeaponUI);
		playerUI->HitEntity = GetSceneManager()->GetEntityByIdentityName(playerUI->HitUI);
		playerUI->InterectionEntity = GetSceneManager()->GetEntityByIdentityName(playerUI->InterectionUI);

		SettingPlayerUI(0);
	}
}
void PlayerUISystem::Finalize()
{
}

void PlayerUISystem::UpdateHP()
{
	auto playerUI = m_PlayerUI.lock()->GetComponent<PlayerUIComponent>();
	if (!playerUI->HPGageEntity.lock())
		return;
	auto playercomp = playerUI->PlayerEntity.lock()->GetComponent<PlayerComponent>();
	auto ui = playerUI->HPGageEntity.lock()->GetComponent<ImageComponent>();
	ui->RightPercent = 1.f - static_cast<float>(playercomp->HP) / static_cast<float>(playercomp->MaxHP);
}

void PlayerUISystem::OnUpdateHP(std::any none)
{
	if (!m_PlayerUI.lock())
		return;
	auto playerUI = m_PlayerUI.lock()->GetComponent<PlayerUIComponent>();

	if (!playerUI->HPBackEntity.lock() || !playerUI->HPGageEntity.lock() || !playerUI->HPBoarderEntity.lock())
		return;
	if (playerUI->HPBackEntity.lock()->HasComponent<ImageBounceComponent>() &&
		playerUI->HPGageEntity.lock()->HasComponent<ImageBounceComponent>() &&
		playerUI->HPBoarderEntity.lock()->HasComponent<ImageBounceComponent>())
	{
		playerUI->HPBackEntity.lock()->GetComponent<ImageBounceComponent>()->AddedBounce = true;
		playerUI->HPGageEntity.lock()->GetComponent<ImageBounceComponent>()->AddedBounce = true;
		playerUI->HPBoarderEntity.lock()->GetComponent<ImageBounceComponent>()->AddedBounce = true;
	}
	UpdateHP();
}

void PlayerUISystem::UpdateVPState()
{
	if (!m_PlayerUI.lock())
		return;

	auto playerUI = m_PlayerUI.lock()->GetComponent<PlayerUIComponent>();
	if (!playerUI->VPeyeEntity.lock())
		return;
	auto playercomp = playerUI->PlayerEntity.lock()->GetComponent<PlayerComponent>();
	auto& imagecomp = *playerUI->VPeyeEntity.lock()->GetComponent<ImageComponent>();
	auto& VPUI = *imagecomp.GetComponent<VPUIComponent>();
	VPMath::Color White{ 1,1,1,1 };
	VPMath::Color temp{};
	float percent = playercomp->VPGageProgress / playercomp->VPGageCoolTime;
	if (percent > 1)
		percent = 1;
	temp = VPMath::Color::Lerp(White, VPUI.ChangeColor, percent);
	if (percent >= 1 || playercomp->ReadyToTransform)
	{
		if (imagecomp.HasComponent<ImageBounceComponent>())
		{
			imagecomp.GetComponent<ImageBounceComponent>()->AddedBounce = true;
		}
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
void PlayerUISystem::OnUpdateVPState(std::any none)
{

	UpdateVPState();
}
void PlayerUISystem::UpdateAim()
{
	auto playerUI = m_PlayerUI.lock()->GetComponent<PlayerUIComponent>();
	if (!playerUI->AimEntity.lock())
		return;

	auto playercomp = playerUI->PlayerEntity.lock()->GetComponent<PlayerComponent>();

	auto& imagecomp = *playerUI->AimEntity.lock()->GetComponent<ImageComponent>();
	auto& AimComp = *imagecomp.GetComponent<AimUIComponent>();
	auto entity = GetSceneManager()->GetEntity(playercomp->SearchedItemID);
	imagecomp.Color.w = 1;
	if (!entity)
		imagecomp.Color.w = 0;
	else
	{
		if (entity->HasComponent<InterectiveComponent>() && entity->GetComponent<InterectiveComponent>()->IsInterective)
		{
			imagecomp.TexturePath = AimComp.Interected;
		}
		else if (entity->HasComponent<EnemyComponent>())
			imagecomp.TexturePath = AimComp.Aimed;
		else
			imagecomp.Color.w = 0;

	}
}


// OnChangeWeapon 이벤트를 만들까..
void PlayerUISystem::UpdateWeaponUI()
{
	/// 총을 가지고 있을 때 ------------------------------------------------------------------------
	auto playerUI = m_PlayerUI.lock()->GetComponent<PlayerUIComponent>();
	if (!playerUI->WeaponEntity.lock())
		return;

	auto playercomp = playerUI->PlayerEntity.lock()->GetComponent<PlayerComponent>();
	auto& ammoUIComp = *playerUI->WeaponEntity.lock()->GetComponent<TextComponent>();
	auto& weaponUIComp = *ammoUIComp.GetComponent<ImageComponent>();
	std::string newweapon{};
	if (playercomp->IsVPMode)
	{
		ammoUIComp.Text = L" ";
		newweapon = "WeaponUI_VPHand.png";
	}
	else if (!playercomp->HasGun)
	{
		ammoUIComp.Text = L" ";
		newweapon = "WeaponUI_Sword.png";
	}

	if (!newweapon.empty())
	{
		if (weaponUIComp.TexturePath != newweapon)
		{
		weaponUIComp.TexturePath = newweapon;
			if (weaponUIComp.HasComponent<ImageBounceComponent>())
			{
				auto imagebounce = weaponUIComp.GetComponent<ImageBounceComponent>();
				imagebounce->AddedBounce = true;
				return;
			}
		}
		return;
	}

	auto& guncomp = *m_SceneManager.lock()->GetComponent<GunComponent>(playercomp->GunEntityID);

	// 들고 있는 총기의 총알 갯수 업데이트
	auto ammoStr = Util::ToWideChar(guncomp.CurrentBullet);
	ammoUIComp.Text = ammoStr;

	// 들고 있는 무기 종류에 따른 총기 이미지 변경
	switch (guncomp.Type)
	{
	case VisPred::Game::GunType::PISTOL:
	{
		newweapon = "WeaponUI_Pistol.png";
		break;
	}
	case VisPred::Game::GunType::RIFLE:
	{
		newweapon = "WeaponUI_Rifle.png";
		break;
	}
	case VisPred::Game::GunType::SHOTGUN:
	{
		newweapon = "WeaponUI_Shotgun.png";
		break;
	}
	default:
		break;
	}

	if (weaponUIComp.TexturePath != newweapon)
	{
		weaponUIComp.TexturePath = newweapon;
		if (weaponUIComp.HasComponent<ImageBounceComponent>())
		{
			auto imagebounce = weaponUIComp.GetComponent<ImageBounceComponent>();
			imagebounce->AddedBounce = true;
			return;
		}
	}



}

void PlayerUISystem::OnUpdateWeaponUI(std::any data)
{
	if (!m_PlayerUI.lock())
		return;
	auto playerUI = m_PlayerUI.lock()->GetComponent<PlayerUIComponent>();
	if (!playerUI->WeaponEntity.lock())
		return;

	playerUI->WeaponEntity.lock()->HasComponent<TextBounceComponent>();
	if (playerUI->WeaponEntity.lock()->HasComponent<TextBounceComponent>())
	{
		playerUI->WeaponEntity.lock()->GetComponent<TextBounceComponent>()->AddedBounce = true;
	}
	UpdateWeaponUI();
}

void PlayerUISystem::SettingPlayerUI(float deltatime)
{
	if (!m_PlayerUI.lock())
	{
		return;
	}
	auto playerUI = m_PlayerUI.lock()->GetComponent<PlayerUIComponent>();
	if (!playerUI->PlayerEntity.lock())
		return;
	UpdateVPState();
	UpdateFadeUI();
	UpdateHitUI(deltatime);
	UpdateAim();
	UpdateWeaponUI();
	UpdateHP();
}
void PlayerUISystem::UpdateFadeUI()
{
	auto playerUI = m_PlayerUI.lock()->GetComponent<PlayerUIComponent>();
	if (!playerUI->FadeEntity.lock())
		return;
	auto playercomp = playerUI->PlayerEntity.lock()->GetComponent<PlayerComponent>();
	if (playercomp->CurrentFSM == VisPred::Game::PlayerFSM::DIE || playercomp->CurrentFSM == VisPred::Game::PlayerFSM::DIE_END)
	{
		auto& fadeui = *playerUI->FadeEntity.lock()->GetComponent<ImageComponent>();
		fadeui.Color.w = playercomp->DieProgress * 2 / playercomp->DieTime;
	}
	else
	{
		auto& fadeui = *playerUI->FadeEntity.lock()->GetComponent<ImageComponent>();
		fadeui.Color.w = Fade_in_out_Percent(playercomp->TransformationProgress, playercomp->TransformationTime);
	}
}
void PlayerUISystem::UpdateHitUI( float deltatime)
{
	auto playerUI = m_PlayerUI.lock()->GetComponent<PlayerUIComponent>();
	if (!playerUI->HitEntity.lock())
		return;


	auto playercomp = playerUI->PlayerEntity.lock()->GetComponent<PlayerComponent>();
	auto& hitui = *playerUI->HitEntity.lock()->GetComponent<ImageComponent>();
	auto& hitcomp = *hitui.GetComponent<HitUIComponent>();

	if (hitcomp.IsHitUIOn)
	{
		hitcomp.ProgressTime += deltatime;
		float halfDuration = hitcomp.DurationTime / 2.0f;

		if (hitcomp.ProgressTime <= halfDuration)
		{
			// 첫 번째 구간: 0에서 0.5까지 선형적으로 증가
			float progressRatio = hitcomp.ProgressTime / halfDuration;
			hitui.Color.w = progressRatio * 0.5f;
		}
		else if (hitcomp.ProgressTime <= hitcomp.DurationTime)
		{
			// 두 번째 구간: 0.5에서 0까지 선형적으로 감소
			float progressRatio = (hitcomp.ProgressTime - halfDuration) / halfDuration;
			hitui.Color.w = 0.5f * (1.0f - progressRatio);
		}
		else
		{
			hitui.Color.w = 0;
			hitcomp.IsHitUIOn = false;
			hitcomp.ProgressTime = 0;
		}
	}
}

double PlayerUISystem::Fade_in_out_Percent(float progress, float totalTime)
{
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
double PlayerUISystem::Fade_in_Percent(float progress, float totalTime)
{
	// 전체 시간이 0 이하일 경우 0을 반환하여 나누기 오류를 방지합니다.
	if (totalTime <= 0.0f) return 0.0;
	// 진행도를 퍼센트로 계산합니다.
	float percentage = progress / totalTime;
	// 0% - 100%에서 0에서 2로 선형적으로 증가
	if (percentage <= 1.0f) 
		return percentage * 2.0;
	// 진행도가 100%를 초과할 경우 2를 반환
	return 2.0;
}
double PlayerUISystem::Fade_out_Percent(float progress, float totalTime)
{
	// 전체 시간이 0 이하일 경우 0을 반환하여 나누기 오류를 방지합니다.
	if (totalTime <= 0.0f) return 0.0;
	// 진행도를 퍼센트로 계산합니다.
	float percentage = progress / totalTime;
	// 0% - 100%에서 2에서 0으로 선형적으로 감소
	if (percentage <= 1.0f) 
		return 2.0 * (1.0f - percentage);
	// 진행도가 100%를 초과할 경우 기본적으로 0을 반환
	return 0.0;
}
bool PlayerUISystem::ResetInterectionUI(std::shared_ptr<Entity> interectentity)
{
	auto& textUI = *interectentity->GetComponent<TextComponent>();
	auto& imageUI = *textUI.GetComponent<ImageComponent>();
	textUI.Color.w = 0;
	imageUI.Color.w = 0;
	return true;
}
void PlayerUISystem::OnUpdateSearchUI(std::any data)
{
	auto entity = GetSceneManager()->GetEntityByIdentityName("PlayerUI");
	if (!entity)
		return;
	UpdateInterectionUI(*entity->GetComponent<PlayerUIComponent>());

}
void PlayerUISystem::OnResetInterectionUI(std::any null)
{
	auto entity = GetSceneManager()->GetEntityByIdentityName("PlayerUI");
	if (!entity)
		return;
	ResetInterectionUI(entity->GetComponent<PlayerUIComponent>()->InterectionEntity.lock());
}
void PlayerUISystem::OnHideBullet(std::any null)
{
	if (!m_PlayerUI.lock())
		return;
	auto playerUI = m_PlayerUI.lock()->GetComponent<PlayerUIComponent>();
	if (!playerUI->WeaponEntity.lock())
		return;

	if (playerUI->WeaponEntity.lock()->HasComponent<TextComponent>())
	{
		playerUI->WeaponEntity.lock()->GetComponent<TextComponent>()->Color.w = 0;
	}

}
void PlayerUISystem::UpdateInterectionUI(PlayerUIComponent& playerUI)
{
	if (!playerUI.InterectionEntity.lock())
		return;
	auto playercomp = playerUI.PlayerEntity.lock()->GetComponent<PlayerComponent>();
	auto& textUI = *playerUI.InterectionEntity.lock()->GetComponent<TextComponent>();
	auto& imageUI = *playerUI.InterectionEntity.lock()->GetComponent<ImageComponent>();

	if (!GetSceneManager()->HasEntity(playercomp->SearchedItemID))
	{
		ResetInterectionUI(playerUI.InterectionEntity.lock());
		return;
	}
	auto entity = GetSceneManager()->GetEntity(playercomp->SearchedItemID).get();
	if (!entity)
		return;

	if (InterectingGun(playerUI.InterectionEntity.lock(), entity)) {}
	else if (InterectingObject(playerUI.InterectionEntity.lock(), entity))
	{
	}

}
bool PlayerUISystem::InterectingGun(std::shared_ptr<Entity> interectionentity, Entity* selectedentity)
{
	if (!selectedentity->HasComponent<GunComponent>())
		return false;

	auto guncomp = selectedentity->GetComponent<GunComponent>();
	auto& textUI = *interectionentity->GetComponent<TextComponent>();
	auto& imageUI = *textUI.GetComponent<ImageComponent>();

	textUI.Color.w = 1;
	imageUI.Color.w = 0.3;

	switch (guncomp->Type)
	{
	case VisPred::Game::GunType::PISTOL:
	{
		textUI.Text = L"Pistol";
		return true;
		break;
	}
	case VisPred::Game::GunType::SHOTGUN:
	{
		textUI.Text = L"Shotgun";
		return true;

		break;
	}
	case VisPred::Game::GunType::RIFLE:
	{
		textUI.Text = L"Rifle";
		return true;

		break;
	}
	default:
		return false;

		break;
	}
}

bool PlayerUISystem::InterectingObject(std::shared_ptr<Entity> interectionentity, Entity* selectedentity)
{
	if (!selectedentity->HasComponent<InterectiveComponent>())
		return false;
	auto& textUI = *interectionentity->GetComponent<TextComponent>();
	auto& imageUI = *textUI.GetComponent<ImageComponent>();

	textUI.Color.w = 1;
	imageUI.Color.w = 0.3;
	textUI.Text = L"press \"F\" Interect";
	return true;
}

void PlayerUISystem::OnDamaged(std::any entityid_Damage)
{
	if (!m_PlayerUI.lock())
		return;
	auto [entityid, damage] = std::any_cast<std::pair<uint32_t, int>>(entityid_Damage);
	auto entity = GetSceneManager()->GetEntity(entityid);
	if (!entity || !entity->HasComponent<PlayerComponent>())
		return;
	auto playerUIComp = m_PlayerUI.lock()->GetComponent<PlayerUIComponent>();
	OnUpdateHP({});

	if (playerUIComp->HitEntity.lock())
	{
		playerUIComp->HitEntity.lock()->GetComponent<HitUIComponent>()->IsHitUIOn = true;
	}


}


void PlayerUISystem::OnShoot(std::any entityID)
{
	OnUpdateWeaponUI();
}
