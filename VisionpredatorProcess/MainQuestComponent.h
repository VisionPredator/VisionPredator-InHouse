#pragma once
#include "Component.h"
#include "QuestComponent.h"
struct MainQuestComponent:
    public Component
{
	VP_JSONBODY(MainQuestComponent, IsSkipMode, QuestSequence, SounKey_Subquest, SounKey_Mainquest, Volume_Subquest, Volume_Mainquest);
    bool IsSkipMode{};
    std::vector < std::string > QuestSequence;
	std::list < QuestComponent* > Questptrs;
    std::string SounKey_Subquest = "QuestClear";
    std::string SounKey_Mainquest = "QuestClear";
    int Volume_Subquest = 20;
    int Volume_Mainquest =20;
};

