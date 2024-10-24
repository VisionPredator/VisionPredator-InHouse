#include "pch.h"
#include "ControllerMovementSystem.h"
#include "../PhysxEngine/VPPhysicsStructs.h"
#include "Components.h"

ControllerMovementSystem::ControllerMovementSystem(std::shared_ptr<SceneManager> sceneManager):System(sceneManager)
{
}

void ControllerMovementSystem::Update(float deltaTime)
{
	///전체적인 컨트롤러 시스템 업데이트


	///인풋을 통해 데이터를 저장.
}

void ControllerMovementSystem::ControllerMoveCalculate(float deltaTime, ControllerComponent& comp)
{

    // Normalize input direction
    if (comp.InputDir.Length() > 0.0f)
    {
        comp.InputDir.Normalize();
    }
    VPMath::Vector3 dir = comp.InputDir;
    // Set appropriate acceleration and friction values
    float acceleration = comp.IsFall ? comp.JumpXZAcceleration : comp.Acceleration;
    float friction = comp.IsFall ? comp.JumpXZDeceleration : comp.DynamicFriction;
    float staticFriction = comp.StaticFriction;
    float gravity = comp.GravityWeight;
    float maxSpeed = comp.MaxSpeed;

    // Apply friction
    if (!comp.IsFall)
    {
        if (dir.x == 0)
			comp.Velocity.x = std::lerp(comp.Velocity.x, 0.f, staticFriction * deltaTime);
		else
			comp.Velocity.x = std::lerp(comp.Velocity.x, 0.f, friction * deltaTime);

		if (dir.z == 0)
			comp.Velocity.z = std::lerp(comp.Velocity.z, 0.f, staticFriction * deltaTime);
		else
			comp.Velocity.z = std::lerp(comp.Velocity.z, 0.f, friction * deltaTime);

		comp.Velocity.x += (dir.x * acceleration * deltaTime);
		comp.Velocity.z += (dir.z * acceleration * deltaTime);
	}
	else
	{
        if (dir.x == 0)
            comp.Velocity.x = std::lerp(comp.Velocity.x, 0.f, friction);
        if (dir.z == 0)
            comp.Velocity.z = std::lerp(comp.Velocity.z, 0.f, friction);


        comp.Velocity.x += (dir.x * acceleration * deltaTime);
        comp.Velocity.z += (dir.z * acceleration * deltaTime);
	}



	// Apply gravity
	if (comp.IsFall)
		comp.Velocity.y -= gravity * deltaTime;

	// Apply jump
	if (comp.InputDir.y != 0 && !comp.IsFall)
		comp.Velocity.y = comp.JumpSpeed;

	VPMath::Vector2 xzVelocity{ comp.Velocity.x, comp.Velocity.z };
	comp.Speed = xzVelocity.Length();


	// Calculate speed and normalize velocity if necessary

    if (comp.Speed > maxSpeed)
    {
        comp.Speed = maxSpeed;
        xzVelocity.Normalize();
        xzVelocity *= comp.Speed;
        comp.Velocity.x = xzVelocity.x;
        comp.Velocity.z = xzVelocity.y;
    }
    else if (comp.Speed < 0.001f)
    {
        comp.Velocity.x = 0;
        comp.Velocity.z = 0;
    }
    else
    {
        xzVelocity.Normalize();
        xzVelocity *= comp.Speed;
        comp.Velocity.x = xzVelocity.x;
        comp.Velocity.z = xzVelocity.y;
    }

    // Reset input direction and check fall state
    //comp.InputDir = {};
    comp.IsFall = m_PhysicsEngine->GetControllerIsFall(comp.GetEntityID());
}



void ControllerMovementSystem::PhysicsUpdate(float deltaTime)
{
    ///인풋에 관련 하여 데이터 시스템을 업데이트 순서  ControllerMoveMentSystem -> Physicsysytem
    for (ControllerComponent& comp : COMPITER(ControllerComponent))
    {
        ControllerMoveCalculate(deltaTime, comp);
        m_PhysicsEngine->SetControllerVelocity(comp.GetEntityID(), comp.Velocity);
    }
}

void ControllerMovementSystem::PhysicsLateUpdate(float deltaTime)
{
}
