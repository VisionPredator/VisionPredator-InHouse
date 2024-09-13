#pragma once
#include "Component.h"
#include "VisPredStructs.h"
struct EnemyComponent :public Component
{
	VP_JSONBODY(EnemyComponent, HP, CurrentFSM, EnemyType)
	float HP{}; 
	VisPred::Game::EFSM CurrentFSM= VisPred::Game::EFSM::IDLE;
	VisPred::Game::GunType EnemyType= VisPred::Game::GunType::PISTOL;
};

