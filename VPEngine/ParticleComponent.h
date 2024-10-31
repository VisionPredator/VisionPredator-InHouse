#pragma once
#include "Component.h"
#include "../VPGraphics/vpGraphics.h"

using ShapeType = effect::ParticleInfo::ParticleShape::ShapeType;
using ParticleBillBoardType = effect::ParticleInfo::ParticleRenderer::ParticleBillBoardType;

struct ParticleComponent : Component
{
	ParticleComponent() = default;

	VP_JSONBODY(ParticleComponent, TexturePath, IsLoop, Restart, Duration, StartLifetime, StartSize, StartSpeed, Shape, RenderMode);

	// TODO: Base �ؽ�ó Path ����ֱ�. �ƴϸ� ParticleObject �󿡼� null�̸� �б�Ÿ���� �ٲٱ�.
	std::string TexturePath;
	bool IsLoop = true;
	bool Restart = false;
	float Duration = 5.f; // ��ƼŬ �ý����� ��ƼŬ�� emit �ϴ� �ð��� ����
	VPMath::Vector2 StartLifetime = { 1.f, 1.f };
	VPMath::Vector2 StartSize = { 3.f, 3.f };
	VPMath::Vector2 StartSpeed = { 5.f, 5.f };

	ShapeType Shape = ShapeType::Cone;
	ParticleBillBoardType RenderMode = ParticleBillBoardType::Billboard;

	bool RestartPrev = false;
};
