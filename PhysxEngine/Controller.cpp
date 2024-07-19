#include "pch.h"
#include "Controller.h"

Controller::Controller()
{
}

Controller::~Controller()
{
}

bool Controller::InfoInit(VPPhysics::ControllerInfo info)
{
	m_EntityID = info.EntityId;
	m_LayerNum= info.LayerNumber;

	return true;
}

