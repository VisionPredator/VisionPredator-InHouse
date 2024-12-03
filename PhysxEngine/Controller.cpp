#include "pch.h"
#include "Controller.h"
#include "ControllerQueryFilterCallback.h"

// Controller 클래스의 생성자
Controller::Controller()
{
}

// Controller 클래스의 소멸자
Controller::~Controller()
{
    // m_Controller가 존재하면 PhysX 객체를 해제
    PX_RELEASE(m_Controller);
    m_Material = nullptr;
}

// 컨트롤러 초기화 함수
// 컨트롤러 정보(info), 물리 재질(material), 물리 설정 정보(physicsinfo)를 기반으로 초기화
bool Controller::ControllerInit(VPPhysics::ControllerInfo info, physx::PxMaterial* material, VPPhysics::PhysicsInfo physicsinfo)
{
    // 컨트롤러의 물리 레이어를 설정
    m_LayerNum = info.LayerNumber;

    // TODO: 원인을 찾아보기 (필터 데이터 초기화 관련)
    m_FilterData = std::make_shared<PxFilterData>();
    m_FilterData->word0 = 0; // 기본적으로 word0을 0으로 설정

    // 새 PxFilterData 생성 및 초기화
    std::shared_ptr<physx::PxFilterData> data = std::make_shared<physx::PxFilterData>();
    m_Material = material;
    data->word0 = (int)m_LayerNum; // 레이어 번호를 word0에 설정
    data->word1 = physicsinfo.CollisionMatrix[(int)m_LayerNum]; // 충돌 매트릭스를 word1에 설정

    // PxQueryFilterCallback 및 PxControllerFilterCallback 생성
    m_PxQueryFilterCallback = std::make_shared<ControllerQueryFilterCallback>(data);
    m_PxControllerFilterCallback = std::make_shared<MyControllerFilterCallback>();

    // PxControllerFilters를 생성하여 필터 콜백 및 데이터 연결
    m_Filters = std::make_shared<PxControllerFilters>(m_FilterData.get(), m_PxQueryFilterCallback.get(), m_PxControllerFilterCallback.get());

    // 사용자 데이터에 엔티티 ID를 설정
    m_UserData.entityID = info.EntityId;

    return true; // 초기화 성공
}

// 컨트롤러의 위치를 설정하는 함수
void Controller::SetPosition(const VPMath::Vector3& position)
{
    // VPMath::Vector3를 PxExtendedVec3로 변환
	physx::PxExtendedVec3 vector{ position.x,position.y,position.z };
    // 컨트롤러의 위치 설정
    m_Controller->setPosition(vector);
}

// 컨트롤러의 현재 위치를 가져오는 함수
const VPMath::Vector3 Controller::GetPosition()
{
	VPMath::Vector3 Pos = {
		static_cast<float>(m_Controller->getPosition().x)
		,static_cast<float>(m_Controller->getPosition().y)
		,static_cast<float>(m_Controller->getPosition().z)
	};
	return Pos; // 변환된 위치 반환
}
