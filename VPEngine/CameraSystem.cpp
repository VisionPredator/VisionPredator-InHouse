#include "pch.h"
#include "Components.h"
#include "CameraSystem.h"

namespace VisPred
{
	void VisPred::CameraSystem::Update(float deltaTime)
	{
		for (CameraComponent& cameracomp : CompIter<CameraComponent>(m_EntityManager))
		{
		}
	}
}