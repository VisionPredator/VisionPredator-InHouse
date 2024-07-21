#include "pch.h"
#include "Controller.h"

Controller::Controller()
{
}

Controller::~Controller()
{
	delete m_FilterData;
}

bool Controller::ControllerInit(VPPhysics::ControllerInfo info, VPPhysics::PhysicsInfo physicsinfo)
{
	m_LayerNum = info.LayerNumber;
	m_FilterData = new physx::PxFilterData;
	m_FilterData->word0 = (int)m_LayerNum;
	m_FilterData->word1 = physicsinfo.CollisionMatrix[(int)m_LayerNum];

	m_EntityID = info.EntityId;

	return true;
}

