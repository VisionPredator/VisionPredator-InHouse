#include "pch.h"
#include "ControllerQueryFilterCallback.h"

ControllerQueryFilterCallback::ControllerQueryFilterCallback(physx::PxFilterData* filterData) : m_FilterData(filterData)
{
}

PxQueryHitType::Enum ControllerQueryFilterCallback::preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags)
{
	if (shape == nullptr || m_FilterData == nullptr)
	{
		return PxQueryHitType::eNONE;
	}

	auto shapeCount = actor->getNbShapes();
	assert(shapeCount >= 1);

	if (shapeCount == 0)
	{
		return PxQueryHitType::eNONE;
	}

	
	PxShape* curShape;
	actor->getShapes(&curShape, 1);

	physx::PxFilterData data = curShape->getSimulationFilterData();

	if (data.word0 > 32)
		return PxQueryHitType::eNONE;

	if ((((1 << m_FilterData->word0) & data.word1) > 0) && (((1 << data.word0) & m_FilterData->word1) > 0))
	{
		return PxQueryHitType::eBLOCK;
	}

	return PxQueryHitType::eNONE;
}

PxQueryHitType::Enum ControllerQueryFilterCallback::postFilter(const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor)
{
	physx::PxFilterData data = shape->getSimulationFilterData();

	if ((((1 << m_FilterData->word0) & data.word1) > 0) && (((1 << data.word0) & m_FilterData->word1) > 0))
	{
		return PxQueryHitType::eBLOCK;
	}

	return PxQueryHitType::eBLOCK;
}

