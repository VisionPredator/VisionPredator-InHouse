#include "pch.h"
#include "RigidBody.h"


#include "pch.h"
#include "RigidBody.h"

// CapsuleColliderInfo를 기반으로 RigidBody 생성자 정의
RigidBody::RigidBody(VPPhysics::CapsuleColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo)
	: m_ColliderType{ type } // 콜라이더 타입 설정
	, m_UserData{ info.colliderInfo.EntityID } // 사용자 데이터 초기화 
	, m_LayerNum{ info.colliderInfo.PhysicsLayer } // 물리 레이어 번호 설정
{
	// 물리 엔진 정보를 저장
	m_PhysicInfo = std::make_shared<VPPhysics::PhysicsInfo>(engininfo);
	// 캡슐 콜라이더 정보 설정
	SetColliderinfo(info);
}

// BoxColliderInfo를 기반으로 RigidBody 생성자 정의
RigidBody::RigidBody(VPPhysics::BoxColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo)
	: m_ColliderType{ type } // 콜라이더 타입 설정
	, m_UserData{ info.colliderInfo.EntityID } // 사용자 데이터 초기화
	, m_LayerNum{ info.colliderInfo.PhysicsLayer } // 물리 레이어 번호 설정
{
	// 물리 엔진 정보를 저장
	m_PhysicInfo = std::make_shared<VPPhysics::PhysicsInfo>(engininfo);
	// 박스 콜라이더 정보 설정
	SetColliderinfo(info);
}

// ConvexColliderInfo를 기반으로 RigidBody 생성자 정의
RigidBody::RigidBody(VPPhysics::ConvexColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo)
	: m_ColliderType{ type } // 콜라이더 타입 설정
	, m_UserData{ info.colliderInfo.EntityID } // 사용자 데이터 초기화 
	, m_LayerNum{ info.colliderInfo.PhysicsLayer } // 물리 레이어 번호 설정
{
	// 물리 엔진 정보를 저장
	m_PhysicInfo = std::make_shared<VPPhysics::PhysicsInfo>(engininfo);
	// Convex 콜라이더 정보 설정
	SetColliderinfo(info);
}

// SphereColliderInfo를 기반으로 RigidBody 생성자 정의
RigidBody::RigidBody(VPPhysics::SphereColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo)
	: m_ColliderType{ type } // 콜라이더 타입 설정
	, m_UserData{ info.colliderInfo.EntityID } // 사용자 데이터 초기화 
	, m_LayerNum{ info.colliderInfo.PhysicsLayer } // 물리 레이어 번호 설정
{
	// 물리 엔진 정보를 저장
	m_PhysicInfo = std::make_shared<VPPhysics::PhysicsInfo>(engininfo);
	// Sphere 콜라이더 정보 설정
	SetColliderinfo(info);
}

// RigidBody 소멸자
RigidBody::~RigidBody() {}
