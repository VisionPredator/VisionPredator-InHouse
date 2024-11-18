#include "pch.h"
#include "GameBaseSystem.h"
#include "GameBaseComponent.h"
#include "PlayerComponent.h"
#include "TextBounceComponent.h"
GameBaseSystem::GameBaseSystem(std::shared_ptr<SceneManager> sceneManager):System(sceneManager)
{
}

void GameBaseSystem::Initialize()
{
}

void GameBaseSystem::Start(uint32_t gameObjectId)
{
	auto entity = GetSceneManager()->GetEntity(gameObjectId);
	if (!entity || !entity->HasComponent<GameBaseComponent>())
		return;
	auto comp =entity->GetComponent<GameBaseComponent>();
	auto player = GetSceneManager()->GetEntityByIdentityName(comp->PlayerName);
	if (!player)
		return;
	if (!player->HasComponent<PlayerComponent>())
		return;

	auto sencitiveUI = GetSceneManager()->GetEntityByIdentityName(comp->SencitiveUI);
	if (comp->MouseSencitive < 0)
		comp->MouseSencitive = 1;
	if (sencitiveUI)
	{
		comp->SencitiveEntity = sencitiveUI;
		auto text = sencitiveUI->GetComponent<TextComponent>();
		text->Color.w = 0;
		
		text->Text = L"마우스 감도 : " + std::to_wstring(comp->MouseSencitive);
	}

	comp->Player = player;
	auto playercomp = comp->Player.lock()->GetComponent<PlayerComponent>();
	playercomp->MaxHP = comp->MaxHP;
	playercomp->HP = comp->MaxHP;
	playercomp->DamageReduce = comp->Reduce;
	playercomp->Sencitive = (static_cast<float>(comp->MouseSencitive) / 2.f / 100.f);

}

void GameBaseSystem::Finish(uint32_t gameObjectId)
{
}

void GameBaseSystem::Finalize()
{
}

void GameBaseSystem::Update(float deltaTime)
{
	COMPLOOP(GameBaseComponent, comp)
	{
		if (!comp.Player.lock())
			continue;

		if (INPUTKEYDOWN(KEYBOARDKEY::RBRACKET))
		{
			comp.MouseSencitive++;
			comp.Player.lock()->GetComponent<PlayerComponent>()->Sencitive = (static_cast<float>(comp.MouseSencitive) / 2.f / 100.f);;
			GetSceneManager()->SerializePrefab(comp.GetEntityID(), true);
			comp.ShowSencitiveUI = true;
			comp.ShowProgress = 0.00f;
		}
		if (INPUTKEYDOWN(KEYBOARDKEY::LBRACKET))
		{
			comp.MouseSencitive--;
			if (comp.MouseSencitive<=0)
				comp.MouseSencitive = 1;

			comp.Player.lock()->GetComponent<PlayerComponent>()->Sencitive = (static_cast<float>(comp.MouseSencitive) / 2.f / 100.f);;
			GetSceneManager()->SerializePrefab(comp.GetEntityID(), true);
			comp.ShowSencitiveUI = true;
			comp.ShowProgress = 0.00f;

		}
		if (INPUTKEYDOWN(KEYBOARDKEY::BACKSLASH))
		{
			comp.MouseSencitive = 20;
			comp.Player.lock()->GetComponent<PlayerComponent>()->Sencitive = (static_cast<float>(comp.MouseSencitive)/2.f / 100.f);;
			GetSceneManager()->SerializePrefab(comp.GetEntityID(), true);
			comp.ShowSencitiveUI = true;
			comp.ShowProgress = 0.00f;
		}

		if (comp.ShowSencitiveUI && comp.SencitiveEntity.lock())
		{
			auto text = comp.SencitiveEntity.lock()->GetComponent<TextComponent>();
			if (comp.ShowProgress <= 0.01f)
			{
				text->Color.w = 1;
				text->Text = L"마우스 감도 : " + std::to_wstring(comp.MouseSencitive);
				if (text->HasComponent<TextBounceComponent>())
					text->GetComponent<TextBounceComponent>()->AddedBounce = true;
			}
			comp.ShowProgress += deltaTime;
			if (comp.ShowProgress > comp.ShowDuration)
			{
				text->Color.w = 0;
				comp.ShowProgress = 0.00f;
				comp.ShowSencitiveUI = false;
			}
		}
		else
			comp.ShowSencitiveUI = false;
	}
}
