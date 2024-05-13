#include "pch.h"
#include "Components.h"
#include "CameraSystem.h"


	void CameraSystem::Update(float deltaTime)
	{
		for (CameraComponent& cameracomp : CompIter<CameraComponent>(m_EntityManager))
		{
		}
	}
