#pragma once
#include "Component.h"
struct EnemyComponent :public Component
{
	VP_JSONBODY(EnemyComponent, HP)
	float HP{};

};

