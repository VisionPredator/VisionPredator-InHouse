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



	///��ǲ�� ���� �����͸� ����.
}

void ControllerMovementSystem::PhysicsUpdate(float deltaTime)
{
///��ǲ�� ���� �Ͽ� ������ �ý����� ������Ʈ ����  ControllerMoveMentSystem -> Physicsysytem
}
