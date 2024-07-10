#include "pch.h"
#include "RigidBodyManager.h"
#include "StaticRigidBody.h"
#include "../VPEngine/EventManager.h"
#include "DynamicRigidBody.h"

RigidBodyManager::RigidBodyManager()
{
	EventManager::GetInstance().Subscribe("OnAddBodyScene",CreateSubscriber(&RigidBodyManager::OnAddBodyScene));
	EventManager::GetInstance().Subscribe("OnReleaseBodyScene",CreateSubscriber(&RigidBodyManager::OnReleaseBodyScene));
}
RigidBodyManager::~RigidBodyManager()
{
	m_RigidBodyMap.clear();	//확인해보기
}


bool RigidBodyManager::Initialize(physx::PxPhysics* physics,physx::PxScene* Scene, CollisionManager* CollManager)
{
	m_Physics = physics;
	m_Scene = Scene;
	m_PhysxCollisionManager = CollManager;
	return true;
}

void RigidBodyManager::Update()
{

}

void RigidBodyManager::CreateStaticBody(const BoxColliderInfo boxinfo, EColliderType collidertype, const PhysicsInfo engininfo)
{
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(boxinfo.colliderInfo.StaticFriction, boxinfo.colliderInfo.DynamicFriction, boxinfo.colliderInfo.Restitution);
	physx::PxShape* shape = m_Physics->createShape(physx::PxBoxGeometry(boxinfo.Extent.x, boxinfo.Extent.y, boxinfo.Extent.z), *pxMaterial);
	StaticRigidBody* rigidBody = SettingStaticBody(shape, boxinfo.colliderInfo, collidertype, engininfo);
	CollisionData* collisiondata = new CollisionData;
	rigidBody->Initialize(boxinfo.colliderInfo,shape,m_Physics, collisiondata);
	m_RigidBodyMap.insert(std::make_pair(boxinfo.colliderInfo.EntityID, rigidBody));
	AddBodyScene(rigidBody);
	shape->release();

}

void RigidBodyManager::CreateStaticBody(const SphereColliderInfo sphereinfo, EColliderType collidertype, const PhysicsInfo engininfo)
{
	// Create the material for the collider
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(sphereinfo.colliderInfo.StaticFriction, sphereinfo.colliderInfo.DynamicFriction, sphereinfo.colliderInfo.Restitution);
	physx::PxShape* shape = m_Physics->createShape(physx::PxSphereGeometry(sphereinfo.Radius), *pxMaterial);
	StaticRigidBody* rigidBody = SettingStaticBody(shape, sphereinfo.colliderInfo, collidertype, engininfo);
	CollisionData* collisiondata = new CollisionData;
	rigidBody->Initialize(sphereinfo.colliderInfo, shape, m_Physics, collisiondata);
	m_RigidBodyMap.insert(std::make_pair(sphereinfo.colliderInfo.EntityID, rigidBody));
	AddBodyScene(rigidBody);
	shape->release();

}
void RigidBodyManager::CreateStaticBody(const CapsuleColliderInfo capsuleinfo, EColliderType collidertype, const PhysicsInfo engininfo)
{
	// Create the material for the collider
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(capsuleinfo.colliderInfo.StaticFriction, capsuleinfo.colliderInfo.DynamicFriction, capsuleinfo.colliderInfo.Restitution);
	// Create the shape for the capsule collider
	physx::PxShape* shape = m_Physics->createShape(physx::PxCapsuleGeometry(capsuleinfo.Radius, capsuleinfo.HalfHeight), *pxMaterial);
	// Set up the static body
	StaticRigidBody* rigidBody = SettingStaticBody(shape, capsuleinfo.colliderInfo, collidertype, engininfo);
	// Create collision data
	CollisionData* collisiondata = new CollisionData;
	// Initialize the rigid body
	rigidBody->Initialize(capsuleinfo.colliderInfo, shape, m_Physics, collisiondata);
	// Insert the rigid body into the map
	m_RigidBodyMap.insert(std::make_pair(capsuleinfo.colliderInfo.EntityID, rigidBody));

	// Add the rigid body to the scene
	AddBodyScene(rigidBody);
	shape->release();

}



StaticRigidBody* RigidBodyManager::SettingStaticBody(physx::PxShape* shape, const ColliderInfo& info, const EColliderType& colliderType, const PhysicsInfo engininfo)
{
	physx::PxFilterData filterdata;
	filterdata.word0 = (int)info.PhysicsLayer;
	filterdata.word1 = engininfo.CollisionMatrix[(int)info.PhysicsLayer];
	shape->setSimulationFilterData(filterdata);

	StaticRigidBody* staticBody = new StaticRigidBody(colliderType, info.EntityID, info.PhysicsLayer);

	m_RigidBodyMap.insert(std::make_pair(staticBody->GetID(), staticBody));
	return staticBody;
}

void RigidBodyManager::OnAddBodyScene(std::any data)
{
	RigidBody* tempRigid = std::any_cast<RigidBody*>(data);

	DynamicRigidBody* dynamicBody = dynamic_cast<DynamicRigidBody*>(tempRigid);
	if (dynamicBody)
		m_Scene->addActor(*dynamicBody->GetPxDynamicRigid());

	StaticRigidBody* StaticBody = dynamic_cast<StaticRigidBody*>(tempRigid);
	if (StaticBody)
		m_Scene->addActor(*StaticBody->GetPxStaticRigid());
}

void RigidBodyManager::OnReleaseBodyScene(std::any data)
{
	auto [rigidBody,IsDynamic] = std::any_cast<std::pair<RigidBody*, bool>>(data);

	if (IsDynamic)
	{
		DynamicRigidBody* dynamicBody = dynamic_cast<DynamicRigidBody*>(rigidBody);
		m_Scene->removeActor(*dynamicBody->GetPxDynamicRigid());
	}
	else
	{
		StaticRigidBody* StaticBody = dynamic_cast<StaticRigidBody*>(rigidBody);
		m_Scene->removeActor(*StaticBody->GetPxStaticRigid());
	}
	// Ensure that the rigidBody is properly deleted
	delete rigidBody;

}

void RigidBodyManager::ReleaseBodyScene(uint32_t entityID)
{
	if (m_RigidBodyMap.find(entityID) == m_RigidBodyMap.end())
		return;

	RigidBody* tempbody = m_RigidBodyMap.find(entityID)->second;
	bool IsDynamic = false;
	if (tempbody)
	{
		DynamicRigidBody* dynamicBody = dynamic_cast<DynamicRigidBody*>(tempbody);
		if (dynamicBody)
		{
			if (dynamicBody->GetPxDynamicRigid()->getScene() == m_Scene)
			{
				m_RigidBodyMap.erase(m_RigidBodyMap.find(entityID));
				IsDynamic = true;
			}
		}
		StaticRigidBody* staticBody = dynamic_cast<StaticRigidBody*>(tempbody);
		if (staticBody)
		{
			if (staticBody->GetPxStaticRigid()->getScene() == m_Scene)
			{
				m_RigidBodyMap.erase(m_RigidBodyMap.find(entityID));
				IsDynamic = false;
			}
		}
	}
	std::any data = std::make_pair(tempbody, IsDynamic);
	EventManager::GetInstance().ScheduleEvent("OnReleaseBodyScene", data);
}

void RigidBodyManager::AddBodyScene(RigidBody* body)
{
	
	EventManager::GetInstance().ScheduleEvent("OnAddBodyScene", body);

}

