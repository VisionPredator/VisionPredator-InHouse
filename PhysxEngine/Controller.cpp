#include "pch.h"
#include "Controller.h"
#include "ControllerQueryFilterCallback.h"


Controller::Controller()
{
}

Controller::~Controller()
{
	PX_RELEASE(m_Controller);
}

bool Controller::ControllerInit(VPPhysics::ControllerInfo info,physx::PxMaterial* material, VPPhysics::PhysicsInfo physicsinfo)
{
	//m_LayerNum = info.LayerNumber;

	///TODO : 원인 찾아보기.
	m_FilterData = std::make_shared<PxFilterData>();
	m_FilterData->word0 = 0;
	std::shared_ptr<physx::PxFilterData> data = std::make_shared<physx::PxFilterData>();
	data->word0 = (int)m_LayerNum;
	m_Material = material;
	data->word1 = physicsinfo.CollisionMatrix[(int)m_LayerNum];
	m_ControllerQueryFilterCallback = std::make_shared<ControllerQueryFilterCallback>(data);
	m_Filters = std::make_shared<PxControllerFilters>(m_FilterData.get(), m_ControllerQueryFilterCallback.get());
	m_EntityID = info.EntityId;

	return true;
}

void Controller::SetPosition(const VPMath::Vector3& position)
{
	physx::PxExtendedVec3 vector;

	vector.x = position.x;
	vector.y = position.y;
	vector.z = position.z;

	m_Controller->setPosition(vector);
}


const VPMath::Vector3 Controller::GetPosition()
{
	VPMath::Vector3 Pos;
	Pos.x=(float) m_Controller->getPosition().x;
	Pos.y= (float)m_Controller->getPosition().y;
	Pos.z= (float)m_Controller->getPosition().z;
	return Pos;
}

