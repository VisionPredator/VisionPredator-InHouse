#pragma once
#include "Component.h"
#include "VisPredStructs.h"
struct EnemyComponent :public Component
{
	VP_JSONBODY(EnemyComponent, HP, CurrentFSM, EnemyType, HorizontalFOV, VerticalFOV, NearZ, FarZ, IsModelFlipped)
	float HP{}; 
	VisPred::Game::EnemyState CurrentFSM= VisPred::Game::EnemyState::Idle;
	VisPred::Game::GunType EnemyType= VisPred::Game::GunType::PISTOL;

	// 시야 관련 변수
	float HorizontalFOV = 60.0f;
	float VerticalFOV = 40.0f;
	float NearZ = 0.2f;
	float FarZ = 13.0f;
	bool IsModelFlipped = true;
};

