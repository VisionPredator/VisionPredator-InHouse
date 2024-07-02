#pragma once
class Physics
{
public:
	Physics();
	~Physics();
	void Initialize();
	void Finalize();
	void SettingPVDClient(physx::PxScene* scene);
	physx::PxPhysics* GetPxPhysics() { return m_Physics; }
	physx::PxDefaultAllocator		m_DefaultAllocatorCallback;
	physx::PxDefaultErrorCallback	m_DefaultErrorCallback;
	physx::PxTolerancesScale		m_ToleranceScale;
	physx::PxDefaultCpuDispatcher* m_Dispatcher;

	// physx
	physx::PxFoundation* m_Foundation;
	physx::PxPhysics* m_Physics;

	// PVD
	physx::PxPvd* m_Pvd;
	
	physx::PxPvdSceneClient* m_PvdClient;
};

