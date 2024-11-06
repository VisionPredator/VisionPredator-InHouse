#pragma once
#include "Component.h"
#include "QuestComponent.h"
struct MainQuestComponent:
    public Component
{
    VP_JSONBODY(MainQuestComponent, QuestSequence);
    std::vector < std::string > QuestSequence;
	std::list < QuestComponent* > Questptrs;
};

