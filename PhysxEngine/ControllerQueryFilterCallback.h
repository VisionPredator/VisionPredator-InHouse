#pragma once
class ControllerQueryFilterCallback:
	public physx::PxQueryFilterCallback
{
public:
	ControllerQueryFilterCallback(std::shared_ptr<physx::PxFilterData> filterData);
	virtual ~ControllerQueryFilterCallback() {}
	// PxQueryFilterCallback을(를) 통해 상속됨
	PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) override;
	PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor) override;

	inline std::shared_ptr<physx::PxFilterData> GetFilterData();
	inline void SetFilterData(std::shared_ptr<physx::PxFilterData> newData);
	std::shared_ptr<physx::PxFilterData> m_FilterData;

};

inline std::shared_ptr<physx::PxFilterData> ControllerQueryFilterCallback::GetFilterData()
{
	return m_FilterData;
}

inline void ControllerQueryFilterCallback::SetFilterData(std::shared_ptr<physx::PxFilterData> newData)
{
	m_FilterData = newData;
}
