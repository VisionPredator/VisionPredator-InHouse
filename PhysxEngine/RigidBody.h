#pragma once
#include "VPPhysicsStructs.h"

class RigidBody
{
public:
    // 다양한 ColliderInfo를 기반으로 RigidBody 생성자 정의
    RigidBody(VPPhysics::CapsuleColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo);
    RigidBody(VPPhysics::BoxColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo);
    RigidBody(VPPhysics::ConvexColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo);
    RigidBody(VPPhysics::SphereColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo);

    // 소멸자를 가상 함수로 선언하여 상속 시 안전하게 사용
    virtual ~RigidBody();

    // 엔터티 ID를 반환하는 함수
    inline const uint32_t GetID() const;

    // 물리 엔진에서 사용할 사용자 데이터 (엔터티 ID, 활성화 여부)
    VPPhysics::USERDATA m_UserData{};
    // 콜라이더 타입 (COLLISION 또는 TRIGGER)
    VPPhysics::EColliderType m_ColliderType{};

    // 물리 레이어 번호
    VPPhysics::EPhysicsLayer m_LayerNum{};

    // 콜라이더 정보 설정 함수 (타입별로 따로 구현)
    void SetColliderinfo(const VPPhysics::CapsuleColliderInfo& capsuleinfo);
    void SetColliderinfo(const VPPhysics::BoxColliderInfo& boxinfo);
    void SetColliderinfo(const VPPhysics::ConvexColliderInfo& convexinfo);
    void SetColliderinfo(const VPPhysics::SphereColliderInfo& sphereinfo);


    // RigidBody 타입 ID를 반환하는 가상 함수 (Reflection 라이브러리를 사용)
	virtual entt::id_type GetTypeID() const { return Reflection::GetTypeID<RigidBody>(); }

    // 콜라이더 정보를 저장하는 스마트 포인터
    std::shared_ptr<VPPhysics::CapsuleColliderInfo> m_Capulseinfo; // 캡슐 콜라이더 정보
    std::shared_ptr<VPPhysics::BoxColliderInfo> m_Boxinfo;         // 박스 콜라이더 정보
    std::shared_ptr<VPPhysics::ConvexColliderInfo> m_Convexinfo;   // Convex 콜라이더 정보
    std::shared_ptr<VPPhysics::SphereColliderInfo> m_Sphereinfo;  // Sphere 콜라이더 정보
    std::shared_ptr<VPPhysics::PhysicsInfo> m_PhysicInfo;         // 물리 엔진 정보
};

// GetID: 엔터티 ID를 반환
const unsigned int RigidBody::GetID() const
{
    return m_UserData.entityID;
}

// SetColliderinfo: 캡슐 콜라이더 정보를 설정
inline void RigidBody::SetColliderinfo(const VPPhysics::CapsuleColliderInfo& capsuleinfo)
{
    // 다른 콜라이더 정보를 초기화
    m_Capulseinfo.reset();
    m_Boxinfo.reset();
    m_Convexinfo.reset();
    m_Sphereinfo.reset();
    // 캡슐 콜라이더 정보 설정
    m_Capulseinfo = std::make_shared<VPPhysics::CapsuleColliderInfo>(capsuleinfo);
}

// SetColliderinfo: 박스 콜라이더 정보를 설정
inline void RigidBody::SetColliderinfo(const VPPhysics::BoxColliderInfo& boxinfo)
{
    // 다른 콜라이더 정보를 초기화
    m_Capulseinfo.reset();
    m_Boxinfo.reset();
    m_Convexinfo.reset();
    m_Sphereinfo.reset();
    // 박스 콜라이더 정보 설정
    m_Boxinfo = std::make_shared<VPPhysics::BoxColliderInfo>(boxinfo);
}

// SetColliderinfo: Convex 콜라이더 정보를 설정
inline void RigidBody::SetColliderinfo(const VPPhysics::ConvexColliderInfo& convexinfo)
{
    // 다른 콜라이더 정보를 초기화
    m_Capulseinfo.reset();
    m_Boxinfo.reset();
    m_Convexinfo.reset();
    m_Sphereinfo.reset();
    // Convex 콜라이더 정보 설정
    m_Convexinfo = std::make_shared<VPPhysics::ConvexColliderInfo>(convexinfo);
}

// SetColliderinfo: Sphere 콜라이더 정보를 설정
inline void RigidBody::SetColliderinfo(const VPPhysics::SphereColliderInfo& sphereinfo)
{
    // 다른 콜라이더 정보를 초기화
    m_Capulseinfo.reset();
    m_Boxinfo.reset();
    m_Convexinfo.reset();
    m_Sphereinfo.reset();
    // Sphere 콜라이더 정보 설정
    m_Sphereinfo = std::make_shared<VPPhysics::SphereColliderInfo>(sphereinfo);
}
