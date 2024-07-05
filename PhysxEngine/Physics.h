#pragma once


class Physics
{
public:
	Physics();
	~Physics();
	void Initialize();
	void Finalize();
	void SettingPVDClient(physx::PxScene* scene);
	inline PxPhysics* GetPxPhysics();
	inline PxDefaultCpuDispatcher* GetDispatcher();
	physx::PxDefaultAllocator		m_DefaultCallback;
	physx::PxDefaultErrorCallback	m_DefaultErrorCallback;
	physx::PxTolerancesScale		m_ToleranceScale{};
	physx::PxDefaultCpuDispatcher* m_Dispatcher;
	// physx
	physx::PxFoundation* m_Foundation;
	physx::PxPhysics* m_Physics;

	// PVD
	physx::PxPvd* m_Pvd;
	physx::PxPvdSceneClient* m_PvdClient;
};

inline physx::PxPhysics* Physics::GetPxPhysics()
{
	return m_Physics;
}

physx::PxDefaultCpuDispatcher* Physics::GetDispatcher()
{
	return m_Dispatcher;
}