#include "pch.h"
#include "ControllerMovementSystem.h"
#include "Components.h"

ControllerMovementSystem::ControllerMovementSystem(SceneManager* sceneManager):System(sceneManager)
{
}

void ControllerMovementSystem::Update(float deltaTime)
{

	COMPLOOP(ControllerComponent)
	{
		




	}



	///인풋을 통해 데이터를 저장.
}

void ControllerMovementSystem::PhysicsUpdate(float deltaTime)
{
///인풋에 관련 하여 데이터 시스템을 업데이트 순서  ControllerMoveMentSystem -> Physicsysytem
}
