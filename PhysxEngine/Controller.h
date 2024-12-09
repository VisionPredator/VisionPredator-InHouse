#pragma once
#include "VPPhysicsStructs.h"
using namespace VPPhysics;

// 컨트롤러 간 필터링을 위한 사용자 정의 필터 콜백 클래스
class ControllerQueryFilterCallback;

// PxControllerFilterCallback를 상속받아 컨트롤러 간 상호작용을 필터링하는 클래스
class MyControllerFilterCallback : public physx::PxControllerFilterCallback
{
public:
    // 두 컨트롤러 a와 b가 상호작용할 수 있는지 여부를 결정하는 함수
    virtual bool filter(const physx::PxController& a, const physx::PxController& b) override {
        // 두 컨트롤러의 Shape를 가져옴 (각 컨트롤러는 하나의 Shape을 가진다고 가정)
        physx::PxShape* shapeA = nullptr;
        physx::PxShape* shapeB = nullptr;
        a.getActor()->getShapes(&shapeA, 1);
        b.getActor()->getShapes(&shapeB, 1);

        // Shape가 하나라도 nullptr이면 상호작용을 차단
        if (!shapeA || !shapeB) {
            return false;
        }

        // Shape의 FilterData를 가져옴
        physx::PxFilterData filterDataA = shapeA->getSimulationFilterData();
        physx::PxFilterData filterDataB = shapeB->getSimulationFilterData();

        // 필터링 논리 (CustomSimulationFilterShader와 유사)
        bool shouldInteract = (((1 << filterDataA.word0) & filterDataB.word1) > 0) &&
            (((1 << filterDataB.word0) & filterDataA.word1) > 0);

        // 두 컨트롤러가 상호작용 가능하면 true 반환
        return shouldInteract;
    }
};

// 컨트롤러 클래스 정의
class Controller
{
public:
    // 기본 생성자 및 소멸자
    Controller();
    ~Controller();

    // 컨트롤러 초기화 함수
    // 정보(info), 재질(material), 물리 정보(physicsinfo)를 입력받아 초기화
    bool ControllerInit(VPPhysics::ControllerInfo info, physx::PxMaterial* material, VPPhysics::PhysicsInfo physicsinfo);

    // 컨트롤러의 위치를 설정하는 함수
    void SetPosition(const VPMath::Vector3& position);

    // 낙하 상태를 설정하는 함수 (inline)
    inline void SetIsFall(bool isfall);

    // 속도를 설정하는 함수 (inline)
    inline void SetVelocity(VPMath::Vector3 velocity);

    // 컨트롤러의 현재 위치를 가져오는 함수
    const VPMath::Vector3 GetPosition();

    // 속도를 가져오는 함수 (inline)
    inline VPMath::Vector3 GetVelocity();

    // 낙하 상태를 가져오는 함수 (inline)
    inline bool GetIsFall();

    // PxController 포인터를 반환하는 함수
    physx::PxController* GetPxController() { return m_Controller; }

    // 컨트롤러 타입 ID를 반환하는 함수
    virtual entt::id_type GetTypeID() const { return Reflection::GetTypeID<Controller>(); }

    // 엔티티 ID를 가져오는 함수 (inline)
    inline uint32_t GetEntityID();

    // 필터를 가져오는 함수 (inline)
    inline physx::PxControllerFilters* GetFilters();

    // 멤버 변수
    USERDATA m_UserData{}; // 사용자 데이터
    physx::PxController* m_Controller{}; // 컨트롤러 객체
    physx::PxControllerCollisionFlags m_Collisionflag; // 충돌 플래그 저장 변수 m_Collisionflag; // 콜리젼 체크
    VPPhysics::EPhysicsLayer m_LayerNum{}; // 물리 레이어 정보
    physx::PxMaterial* m_Material{}; // 물리 재질
    std::shared_ptr<PxFilterData> m_FilterData{}; // 필터 데이터
    std::shared_ptr<ControllerQueryFilterCallback> m_PxQueryFilterCallback{}; // 쿼리 필터 콜백
    std::shared_ptr<MyControllerFilterCallback> m_PxControllerFilterCallback{}; // 컨트롤러 필터 콜백
    std::shared_ptr<PxControllerFilters> m_Filters{}; // 필터

    physx::PxVec3 m_Velocity{}; // 속도
    bool m_IsFall{}; // 낙하 상태
};

// 엔티티 ID를 반환하는 함수 (inline)
inline uint32_t Controller::GetEntityID()
{
    return m_UserData.entityID;
}

// 필터를 반환하는 함수 (inline)
inline physx::PxControllerFilters* Controller::GetFilters()
{
    return m_Filters.get();
}

// 낙하 상태를 설정하는 함수 (inline)
inline void Controller::SetIsFall(bool isfall)
{
    m_IsFall = isfall;
}

// 낙하 상태를 가져오는 함수 (inline)
inline bool Controller::GetIsFall()
{
    return m_IsFall;
}

// 속도를 설정하는 함수 (inline)
inline void Controller::SetVelocity(VPMath::Vector3 velocity)
{
    m_Velocity = { velocity.x, velocity.y, velocity.z };
}

// 속도를 가져오는 함수 (inline)
inline VPMath::Vector3 Controller::GetVelocity()
{
    return { m_Velocity.x, m_Velocity.y, m_Velocity.z };
}
