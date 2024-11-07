#pragma once
#include <Component.h>
#include "PlayerComponent.h"
#include "VisPredStructs.h"

struct QuestComponent :
    public Component
{
    VP_JSONBODY(QuestComponent, PlayerIdentity, QuestType, IsStarted, IsCleared)
    VisPred::Game::QuestType QuestType;
    bool IsStarted{};
    bool IsCleared{};
    std::string PlayerIdentity = "Player";
    PlayerComponent* QuestPlayer;

};

