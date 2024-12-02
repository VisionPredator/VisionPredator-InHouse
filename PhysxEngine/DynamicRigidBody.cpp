#include "pch.h"
#include "DynamicRigidBody.h"
using namespace VPPhysics;

DynamicRigidBody::DynamicRigidBody(BoxColliderInfo info, EColliderType type, const PhysicsInfo& engininfo) :RigidBody(info, type, engininfo) {}
DynamicRigidBody::DynamicRigidBody(CapsuleColliderInfo info, EColliderType type, const PhysicsInfo& engininfo) :RigidBody(info, type, engininfo) {}
DynamicRigidBody::DynamicRigidBody(SphereColliderInfo info, EColliderType type, const PhysicsInfo& engininfo) :RigidBody(info, type, engininfo) {}
DynamicRigidBody::DynamicRigidBody(ConvexColliderInfo info, EColliderType type, const PhysicsInfo& engininfo) :RigidBody(info, type, engininfo) {}

DynamicRigidBody::~DynamicRigidBody()
{
    // RigidBody에 연결된 shape 가져오기
    physx::PxShape* shape;
    m_DynamicRigid->getShapes(&shape, 1); // 첫 번째 shape를 가져옴

    physx::PxMaterial* material = nullptr; // 사용할 Material 포인터 초기화

    if (shape) // shape가 유효한 경우
    {
        // shape에 연결된 material의 개수 가져오기
        PxU32 materialCount = shape->getNbMaterials();
        // material 개수만큼 배열 할당
        PxMaterial** materials = new PxMaterial * [materialCount];
        // shape에 연결된 모든 material 가져오기
        shape->getMaterials(materials, materialCount);
        // shape를 RigidBody에서 분리
        m_DynamicRigid->detachShape(*shape);
        // 모든 material 메모리 해제
        for (PxU32 i = 0; i < materialCount; ++i)
            materials[i]->release(); // material 객체 해제
        // material 배열 삭제
        delete[] materials;
    }
    // RigidBody 객체 해제
    m_DynamicRigid->release();
}

bool DynamicRigidBody::Initialize(physx::PxShape* shape, physx::PxPhysics* physics)
{
    // 콜라이더 타입이 COLLISION인 경우
    if (m_ColliderType == EColliderType::COLLISION)
        // 충돌 시뮬레이션에 사용할 수 있도록 설정
        shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
    else
    {
        // 시뮬레이션 기능 비활성화
        shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
        // 트리거 기능 활성화
        shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
    }
    // shape의 사용자 데이터를 m_UserData로 설정
    shape->userData = &m_UserData;
    // 접촉 오프셋 설정 (충돌 시 인식 거리를 조정)
    shape->setContactOffset(0.02f);
    // 정지 오프셋 설정 (표면 간 간격을 유지)
    shape->setRestOffset(0.01f);
    // 초기화 시 사용할 변환 정보와 충돌 정보 준비
    physx::PxTransform transform;
    VPPhysics::ColliderInfo colliderInfo{};
    // 다양한 콜라이더 타입에 따라 colliderInfo 설정
    if (m_Boxinfo)
        colliderInfo = m_Boxinfo->colliderInfo; // Box 콜라이더 정보
    else if (m_Sphereinfo)
        colliderInfo = m_Sphereinfo->colliderInfo; // Sphere 콜라이더 정보
    else if (m_Convexinfo)
        colliderInfo = m_Convexinfo->colliderInfo; // Convex 콜라이더 정보
    else if (m_Capulseinfo)
        colliderInfo = m_Capulseinfo->colliderInfo; // Capsule 콜라이더 정보
    else
        VP_ASSERT(false, "rigidbody 초기화 오류!"); // 예외 상황 처리

    // 변환 정보 설정 (위치 및 회전)
    transform.p = {
        colliderInfo.WorldLocation.x,
        colliderInfo.WorldLocation.y,
        colliderInfo.WorldLocation.z
    };
    transform.q.x = colliderInfo.WorldQuaternion.x;
    transform.q.y = colliderInfo.WorldQuaternion.y;
    transform.q.z = colliderInfo.WorldQuaternion.z;
    transform.q.w = colliderInfo.WorldQuaternion.w;

    // 동적 강체 생성
    m_DynamicRigid = physics->createRigidDynamic(transform);

    // 중력 사용 여부 설정
    m_DynamicRigid->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !colliderInfo.UseGravity);

    // LOCK 설정 (선형 및 각도 고정)
    m_DynamicRigid->setRigidDynamicLockFlags(
        (PxRigidDynamicLockFlag::Enum)(colliderInfo.LinearLock[0] << 0) | // X축 선형 고정
        (PxRigidDynamicLockFlag::Enum)(colliderInfo.LinearLock[1] << 1) | // Y축 선형 고정
        (PxRigidDynamicLockFlag::Enum)(colliderInfo.LinearLock[2] << 2) | // Z축 선형 고정
        (PxRigidDynamicLockFlag::Enum)(colliderInfo.AngleLock[0] << 3) |  // X축 각도 고정
        (PxRigidDynamicLockFlag::Enum)(colliderInfo.AngleLock[1] << 4) |  // Y축 각도 고정
        (PxRigidDynamicLockFlag::Enum)(colliderInfo.AngleLock[2] << 5));  // Z축 각도 고정

    // 사용자 데이터 설정
    m_DynamicRigid->userData = &m_UserData;

    // 동적 강체 생성 실패 시 false 반환
    if (m_DynamicRigid == nullptr)
        return false;
    // 생성된 강체에 shape 연결
    if (!m_DynamicRigid->attachShape(*shape))
	{
        // 연결 실패 시 참조 카운트가 1인지 확인 후 false 반환
        assert(shape->getReferenceCount() == 1);
        return false;
    }

    // 질량 및 관성 업데이트 (밀도를 사용하여 계산)
    physx::PxRigidBodyExt::updateMassAndInertia(*m_DynamicRigid, colliderInfo.Density);
    return true; // 초기화 성공
}
