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
	// Release Dispatcher
	PX_RELEASE(m_Dispatcher);
	// Release Physics
	PX_RELEASE(m_Physics);
	// Release PVD
	if (m_Pvd)
	{
		// Release PVD Transport
		m_Pvd->release();
		m_Pvd = nullptr;
	}
	// Release Foundation
	PX_RELEASE(m_Foundation);



}

void Physics::Initialize()
{
	// PhysX Foundation�� �����ϰ� �ʱ�ȭ�մϴ�.
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultCallback, m_DefaultErrorCallback);

	// Foundation�� ���������� �����Ǿ����� Ȯ���մϴ�.
	if (!m_Foundation)
		throw("PxCreateFoundation failed!"); // Foundation ������ ������ ��� ���ܸ� throw�մϴ�.

#ifdef _DEBUG
	// PhysX Visual Debugger (PVD)�� �����ϰ� �����մϴ�.
	m_Pvd = physx::PxCreatePvd(*m_Foundation); // Foundation�� ����Ͽ� PVD�� �����մϴ�.
	if (!m_Pvd)
		throw("PxCreatePvd failed!");

	m_pTransport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10); // PVD�� ����� Ʈ������Ʈ�� �����մϴ�.
	if (!m_pTransport)
		throw("PxDefaultPvdSocketTransportCreate failed!");

	bool currentState = m_Pvd->isConnected();
	if (!currentState)
		m_Pvd->connect(*m_pTransport, PxPvdInstrumentationFlag::eALL);

	// ���� �ùķ��̼��� ��� ���� �������� �����մϴ�.
	m_ToleranceScale.length = 1.00; // ���� ��� ���� �������� �����մϴ�.
	m_ToleranceScale.speed = 9.81; // �ӵ� ��� ���� �������� �����մϴ�.

	// PhysX Physics�� �����ϰ� �ʱ�ȭ�մϴ�.
	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true, m_Pvd); // Physics�� �����մϴ�.
	if (!m_Physics)
		throw("PxCreatePhysics failed!");

	// CPU ����ó�� �����ϰ� �����մϴ�.
	m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(3); // CPU ����ó�� �����մϴ�.
	if (!m_Dispatcher)
		throw("PxDefaultCpuDispatcherCreate failed!");

#else
	// ���� �ùķ��̼��� ��� ���� �������� �����մϴ�.
	m_ToleranceScale.length = 100; // ���� ��� ���� �������� �����մϴ�.
	m_ToleranceScale.speed = 981; // �ӵ� ��� ���� �������� �����մϴ�.
	// PhysX Physics�� �����ϰ� �ʱ�ȭ�մϴ�.
	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true, nullptr); // Physics�� �����մϴ�.
	// CPU ����ó�� �����ϰ� �����մϴ�.
	m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(3); // CPU ����ó�� �����մϴ�.
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


