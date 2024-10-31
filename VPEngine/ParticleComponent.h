#pragma once
#include "Component.h"
#include "../VPGraphics/vpGraphics.h"

using ShapeType = effect::ParticleInfo::ParticleShape::ShapeType;
using ParticleBillBoardType = effect::ParticleInfo::ParticleRenderer::ParticleBillBoardType;

struct ParticleComponent : Component
{
	ParticleComponent() = default;

	VP_JSONBODY(ParticleComponent, TexturePath, IsLoop, Restart, Duration, StartLifetime, StartSize, StartSpeed, Shape, RenderMode);

	// TODO: Base 텍스처 Path 집어넣기. 아니면 ParticleObject 상에서 null이면 분기타도록 바꾸기.
	std::string TexturePath;
	bool IsLoop = true;
	bool Restart = false;
	float Duration = 5.f; // 파티클 시스템이 파티클을 emit 하는 시간의 길이
	VPMath::Vector2 StartLifetime = { 1.f, 1.f };
	VPMath::Vector2 StartSize = { 3.f, 3.f };
	VPMath::Vector2 StartSpeed = { 5.f, 5.f };

	ShapeType Shape = ShapeType::Cone;
	ParticleBillBoardType RenderMode = ParticleBillBoardType::Billboard;

	bool RestartPrev = false;
};
