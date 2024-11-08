#pragma once
#include <Component.h>
struct InterectiveComponent :
	public Component
{
	VP_JSONBODY(InterectiveComponent, IsInterective, SearcehdColor, Soundkey, Volume)
		bool IsInterective = true;
	VPMath::Color SearcehdColor = { 1,0,0,1 };
	std::string Soundkey = "Interect";
	int Volume = 30;
};

