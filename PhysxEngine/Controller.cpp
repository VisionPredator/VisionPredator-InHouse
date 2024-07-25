#include "pch.h"
#include "Controller.h"
#include "ControllerQueryFilterCallback.h"


Controller::Controller()
{
}

Controller::~Controller()
{
	PX_RELEASE(m_Controller);
	delete m_FilterData;
}

bool Controller::ControllerInit(VPPhysics::ControllerInfo info,physx::PxMaterial* material, VPPhysics::PhysicsInfo physicsinfo)
{
	m_LayerNum = info.LayerNumber;

	///TODO : 원인 찾아보기.
	m_FilterData = new physx::PxFilterData;
	m_FilterData->word0 = 0;
	//m_FilterData->word0 = (int)m_LayerNum;
	//m_FilterData->word1 = physicsinfo.CollisionMatrix[(int)m_LayerNum];
	physx::PxFilterData* data = new physx::PxFilterData;
	data->word0 = (int)m_LayerNum;
	m_Material = material;
	data->word1 = physicsinfo.CollisionMatrix[(int)m_LayerNum];
	m_ControllerQueryFilterCallback = new ControllerQueryFilterCallback(data);
	m_Filters =new physx::PxControllerFilters(m_FilterData, m_ControllerQueryFilterCallback);
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

