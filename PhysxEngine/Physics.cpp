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
	// PhysX Foundation�� �����ϰ� �ʱ�ȭ�մϴ�.
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback, m_DefaultErrorCallback);

	// Foundation�� ���������� �����Ǿ����� Ȯ���մϴ�.
	if (!m_Foundation)
	{
		throw("PxCreateFoundation failed!"); // Foundation ������ ������ ��� ���ܸ� throw�մϴ�.
	}

#ifdef _DEBUG
	// PhysX Visual Debugger (PVD)�� �����ϰ� �����մϴ�.
	m_Pvd = physx::PxCreatePvd(*m_Foundation); // Foundation�� ����Ͽ� PVD�� �����մϴ�.
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10); // PVD�� ����� Ʈ������Ʈ�� �����մϴ�.
	m_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL); // PVD�� Ʈ������Ʈ�� �����մϴ�.

	// ���� �ùķ��̼��� ��� ���� �������� �����մϴ�.
	m_ToleranceScale.length = 100; // ���� ��� ���� �������� �����մϴ�.
	m_ToleranceScale.speed = 981; // �ӵ� ��� ���� �������� �����մϴ�.
	// PhysX Physics�� �����ϰ� �ʱ�ȭ�մϴ�.
	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true, m_Pvd); // Physics�� �����մϴ�.

	// CPU ����ó�� �����ϰ� �����մϴ�.
	m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(3); // CPU ����ó�� �����մϴ�.
#else
	// ���� �ùķ��̼��� ��� ���� �������� �����մϴ�.
	m_ToleranceScale.length = 100; // ���� ��� ���� �������� �����մϴ�.
	m_ToleranceScale.speed = 981; // �ӵ� ��� ���� �������� �����մϴ�.

	// PhysX Physics�� �����ϰ� �ʱ�ȭ�մϴ�.
	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true, nullptr); // Physics�� �����մϴ�.

	// CPU ����ó�� �����ϰ� �����մϴ�.
	m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2); // CPU ����ó�� �����մϴ�.
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


