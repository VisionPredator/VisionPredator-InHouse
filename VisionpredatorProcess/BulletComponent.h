#pragma once
#include "Component.h"
struct BulletComponent :public Component
{
	VP_JSONBODY(BulletComponent, Damage, Speed)
	float Damage{};
	float Speed{};
};

