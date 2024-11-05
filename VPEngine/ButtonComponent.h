#pragma once
#include "Component.h"
#include "EngineStructs.h"


struct ButtonComponent :
    public Component
{
public:
	VP_JSONBODY(ButtonComponent, skill,name, ChangeImage)
		ButtonComponent();

	
	VisPred::Engine::Skill skill = VisPred::Engine::Skill::SCENECHANGE;
	std::vector<std::string> name;
	std::vector<std::string> ChangeImage;
	bool isActive = true;
	bool isClicked = false;
};

