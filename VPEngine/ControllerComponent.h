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
	float MaxSpeed = 0.025f;							// �̵� �ִ� �ӵ� : ĳ���Ͱ� ������ �� �ִ� �ִ� �ӵ�
	float Acceleration = 1.f;							// ���ӵ� : ĳ���Ͱ� �Է� ���� ���� �� �̵� ���ӵ�
	float StaticFriction = 0.4f;						// ���� ���� ��� : ĳ���Ͱ� �̵� �� ������ �� ĳ���Ͱ� �޴� ������ ( 0.0f ~ 1.f )
	float DynamicFriction = 0.1f;						// ���� ���� ��� : �̵� �߿� ĳ���Ͱ� �޴� ������ ( 0.0f ~ 1.f )
	float JumpSpeed = 0.05f;							// ����(y��) �ӵ�
	float JumpXZAcceleration = 10.f;					// ���� �߿� �̵�(XZ��) ���ӵ� ��
	float JumpXZDeceleration = 0.1f;					// ���� �߿� �̵�(XZ��) ���� �� ( 0.0 ~ 1.0 )
	float GravityWeight = 0.2f;							// �⺻ �߷� ���� �� �� ������ ����ġ�� �� �ְ� ���� �� ���� �ٸ��� ������ �� �ֽ��ϴ�.


};

