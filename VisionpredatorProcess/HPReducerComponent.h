#pragma once
#include "Component.h"
struct HPReducerComponent : public Component
{
	VP_JSONBODY(HPReducerComponent, MaxHP, DownHP)
		int MaxHP= 50;
	int DownHP = 10;
};

