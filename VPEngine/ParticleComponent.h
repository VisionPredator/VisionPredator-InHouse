#pragma once
#include "Component.h"
#include "../VPGraphics/vpGraphics.h"

using ShapeType = effect::ParticleInfo::ParticleShape::ShapeType;
using ParticleBillBoardType = effect::ParticleInfo::ParticleRenderer::ParticleBillBoardType;
using BlendType = effect::ParticleInfo::BlendType;

struct ParticleComponent : Component
{
	ParticleComponent() = default;

	VP_JSONBODY(ParticleComponent, TexturePath, IsRender, IsLoop, Restart, Gravity, Duration, StartLifetime, StartSize, StartSpeed, StartColor, EndColor, Shape, Angle, Radius, RenderMode, BlendMode);

	std::string TexturePath;
	bool IsRender = true;
	bool IsLoop = true;
	bool Restart = false;
	float Progresstime = 0.f;
	bool IsPlaying = false;
	float Gravity = -7.8f;

	float Duration = 5.f; // 파티클 시스템이 파티클을 emit 하는 시간의 길이
	VPMath::Vector2 StartLifetime = { 1.f, 1.f };
	VPMath::Vector2 StartSize = { 3.f, 3.f };
	VPMath::Vector2 StartSpeed = { 5.f, 5.f };

	VPMath::Color StartColor = { 1.f, 1.f, 1.f, 1.f };
	VPMath::Color EndColor = { 1.f, 1.f, 1.f, 1.f };
	    
	ShapeType Shape = ShapeType::Cone;
	float Angle = 30.f;
	float Radius = 1.f;

	ParticleBillBoardType RenderMode = ParticleBillBoardType::Billboard;

	BlendType BlendMode = BlendType::AlphaBlend;


	bool RestartPrev = false;
};
