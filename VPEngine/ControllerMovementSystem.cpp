#include "pch.h"
#include "ControllerMovementSystem.h"
#include "../PhysxEngine/VPPhysicsStructs.h"
#include "Components.h"

ControllerMovementSystem::ControllerMovementSystem(std::shared_ptr<SceneManager> sceneManager):System(sceneManager)
{
}


void ControllerMovementSystem::ControllerMoveCalculate(float deltaTime, ControllerComponent& comp)
{
    // Update fall state before processing movement
    bool wasFalling = comp.IsFall;
    comp.IsFall = m_PhysicsEngine->GetControllerIsFall(comp.GetEntityID());

    // Normalize input direction
    if (comp.InputDir.Length() > 0.0f)
    {
        comp.InputDir.Normalize();
    }
    VPMath::Vector3 dir = comp.InputDir;
    float acceleration = comp.IsFall ? comp.JumpXZAcceleration : comp.Acceleration;
    float friction = comp.IsFall ? comp.JumpXZDeceleration : comp.DynamicFriction;
    float staticFriction = comp.StaticFriction;
    float gravity = comp.GravityWeight;
    float maxSpeed = comp.MaxSpeed;

    // Apply gravity and initialize Y velocity on fall start
    if (comp.IsFall)
    {
        // 처음 떨어지기 시작할 때 Y속도를 0으로 설정
        if (!wasFalling)
        {
            comp.Velocity.y = 0;
        }
        comp.Velocity.y -= gravity * deltaTime;
    }
    else
    {
        comp.Velocity.y = -15.f;
    }

    // Apply jump only if starting the jump (when InputDir.y != 0 and not falling)
    if (comp.InputDir.y != 0 && !comp.IsFall && !wasFalling)
    {
        comp.Velocity.y = comp.JumpSpeed;
        comp.IsFall = true; // 점프를 시작하면서 IsFall 상태를 true로 변경
    }

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

    VPMath::Vector2 xzVelocity{ comp.Velocity.x, comp.Velocity.z };
    comp.Speed = xzVelocity.Length();

    // Cap speed at maxSpeed
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

    // Reset input direction
    comp.InputDir = VPMath::Vector3(0, 0, 0);
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
