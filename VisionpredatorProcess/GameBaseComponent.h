#pragma once
#include "Component.h"
struct GameBaseComponent:public Component
{
	VP_JSONBODY(GameBaseComponent, PlayerName, Reduce, SencitiveUI, MouseSencitive, MaxHP)
		std::string PlayerName = "Player";
		std::string SencitiveUI = "Sencitive";
	int MouseSencitive = 20;
	int MaxHP = 125;
	int Reduce = 0;
	std::weak_ptr<Entity> Player;
	std::weak_ptr<Entity> SencitiveEntity;

	bool ShowSencitiveUI = false;
	float ShowDuration = 1.f;
	float ShowProgress = 0.f;

};

