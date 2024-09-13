#pragma once
#include "Component.h"
#include "../PhysxEngine/VPPhysicsStructs.h"
struct ControllerComponent :
	public Component
{
	VP_JSONBODY(ControllerComponent, Contollerinfo, CapsuleControllerinfo, MaxSpeed, Acceleration, StaticFriction, DynamicFriction, JumpSpeed, JumpXZAcceleration, JumpXZDeceleration, GravityWeight);

	VPPhysics::ControllerInfo Contollerinfo;
	VPPhysics::CapsuleControllerInfo CapsuleControllerinfo;
	VPMath::Vector3 InputDir{};
	VPMath::Vector3 Velocity{};
	float Speed{};
	bool IsFall = true;
	float MaxSpeed = 0.025f;							// 이동 최대 속도 : 캐릭터가 움직일 수 있는 최대 속도
	float Acceleration = 1.f;							// 가속도 : 캐릭터가 입력 값을 받을 때 이동 가속도
	float StaticFriction = 0.4f;						// 정적 마찰 계수 : 캐릭터가 이동 중 멈췄을 때 캐릭터가 받는 마찰력 ( 0.0f ~ 1.f )
	float DynamicFriction = 0.1f;						// 동적 마찰 계수 : 이동 중에 캐릭터가 받는 마찰력 ( 0.0f ~ 1.f )
	float JumpSpeed = 0.05f;							// 점프(y축) 속도
	float JumpXZAcceleration = 10.f;					// 점프 중에 이동(XZ축) 가속도 값
	float JumpXZDeceleration = 0.1f;					// 점프 중에 이동(XZ축) 감속 값 ( 0.0 ~ 1.0 )
	float GravityWeight = 0.2f;							// 기본 중력 값을 줄 수 있지만 가중치를 더 주고 싶을 때 값을 다르게 세팅할 수 있습니다.


};

