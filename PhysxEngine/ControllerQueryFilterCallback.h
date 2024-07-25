#pragma once
class ControllerQueryFilterCallback:
	public physx::PxQueryFilterCallback
{
public:
	ControllerQueryFilterCallback(physx::PxFilterData* filterData );
	virtual ~ControllerQueryFilterCallback() {}
	// PxQueryFilterCallback을(를) 통해 상속됨
	PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) override;
	PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor) override;

	inline PxFilterData* GetFilterData();
	inline void SetFilterData(PxFilterData* newData);

	physx::PxFilterData* m_FilterData;
};

inline PxFilterData* ControllerQueryFilterCallback::GetFilterData()
{
	return m_FilterData;
}

inline void ControllerQueryFilterCallback::SetFilterData(PxFilterData* newData)
{
	m_FilterData = newData;
}
