#pragma once
#include "Component.h"
#include "EngineStructs.h"


struct ButtonComponent :
    public Component
{
public:
	VP_JSONBODY(ButtonComponent, skill)
		ButtonComponent();

	
	VisPred::Engine::Skill skill;
};

