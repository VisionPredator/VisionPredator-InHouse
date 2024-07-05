#include "pch.h"
#include "Physics.h"
using namespace physx;

Physics::Physics()
	: m_Dispatcher(nullptr)
	, m_Foundation(nullptr)
	, m_Physics(nullptr)
	, m_Pvd(nullptr)
	, m_PvdClient(nullptr)
{
}

Physics::~Physics()
{
}

void Physics::Initialize()
{
	// PhysX Foundation을 생성하고 초기화합니다.
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultCallback, m_DefaultErrorCallback);

	// Foundation이 성공적으로 생성되었는지 확인합니다.
	if (!m_Foundation)
	{
		throw("PxCreateFoundation failed!"); // Foundation 생성에 실패한 경우 예외를 throw합니다.
	}

#ifdef _DEBUG
	// PhysX Visual Debugger (PVD)를 생성하고 설정합니다.
	m_Pvd = physx::PxCreatePvd(*m_Foundation); // Foundation을 사용하여 PVD를 생성합니다.
	if (!m_Pvd)
	{
		throw("PxCreatePvd failed!");
	}

	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10); // PVD에 사용할 트랜스포트를 생성합니다.
	if (!transport)
	{
		throw("PxDefaultPvdSocketTransportCreate failed!");
	}

	m_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL); // PVD를 트랜스포트에 연결합니다.#endif

	// 물리 시뮬레이션의 허용 오차 스케일을 설정합니다.
	m_ToleranceScale.length = 100; // 길이 허용 오차 스케일을 설정합니다.
	m_ToleranceScale.speed = 981; // 속도 허용 오차 스케일을 설정합니다.

	// PhysX Physics를 생성하고 초기화합니다.
	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true, m_Pvd); // Physics를 생성합니다.
	if (!m_Physics)
	{
		throw("PxCreatePhysics failed!");
}

	// CPU 디스패처를 생성하고 설정합니다.
	m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2); // CPU 디스패처를 생성합니다.
	if (!m_Dispatcher)
	{
		throw("PxDefaultCpuDispatcherCreate failed!");
	}

#else
	// 물리 시뮬레이션의 허용 오차 스케일을 설정합니다.
	m_ToleranceScale.length = 100; // 길이 허용 오차 스케일을 설정합니다.
	m_ToleranceScale.speed = 981; // 속도 허용 오차 스케일을 설정합니다.

	// PhysX Physics를 생성하고 초기화합니다.
	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true, nullptr); // Physics를 생성합니다.

	// CPU 디스패처를 생성하고 설정합니다.
	m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2); // CPU 디스패처를 생성합니다.
#endif
}

void Physics::SettingPVDClient(physx::PxScene* scene)
{
	m_PvdClient = scene->getScenePvdClient();
	if (m_PvdClient)
	{
		m_PvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		m_PvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		m_PvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
}



void Physics::Finalize()
{
}


