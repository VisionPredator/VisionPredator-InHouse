#pragma once
#include "Component.h"
#include "EngineStructs.h"


struct ButtonComponent :
    public Component
{
public:
	VP_JSONBODY(ButtonComponent, skill,name)
		ButtonComponent();

	
	VisPred::Engine::Skill skill = VisPred::Engine::Skill::SCENECHANGE;
	std::vector<std::string> name;
	bool isActive = true;
	bool isClicked = false;
};

