#pragma once
#include <Component.h>
struct ParticleOwnerComponent :
	public Component
{
	VP_JSONBODY(ParticleOwnerComponent, ParticleName);
	std::string ParticleName;

};

