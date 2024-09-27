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

void PlayerUISystem::Initialize()
{
}

void PlayerUISystem::Start(uint32_t gameObjectId)
{
}

void PlayerUISystem::Finish(uint32_t gameObjectId)
{
}

void PlayerUISystem::Finalize()
{
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
	// else if 상호작용 중일 때.
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

// OnChangeWeapon 이벤트를 만들까..
void PlayerUISystem::UpdateWeapon(const PlayerComponent& playerComponent)
{

}

void PlayerUISystem::OnGunShoot(std::any data)
{

}
