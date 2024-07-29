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


bool RigidBodyManager::Initialize(physx::PxPhysics* physics,physx::PxScene* Scene)
{
	m_Physics = physics;
	m_Scene = Scene;
	return true;
}

void RigidBodyManager::Update()
{

}

void RigidBodyManager::CreateStaticBody(const BoxColliderInfo boxinfo, EColliderType collidertype, const PhysicsInfo engininfo)
{
	// Create the material for the box
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(boxinfo.colliderInfo.StaticFriction, boxinfo.colliderInfo.DynamicFriction, boxinfo.colliderInfo.Restitution);
	// Create the shape for the box collider
	physx::PxShape* shape = m_Physics->createShape(physx::PxBoxGeometry(boxinfo.Extent.x, boxinfo.Extent.y, boxinfo.Extent.z), *pxMaterial);
	// Set up the static body
	StaticRigidBody* rigidBody = SettingStaticBody(shape, boxinfo.colliderInfo, collidertype, engininfo);
	// Create collision data
	//CollisionData* collisiondata = new CollisionData;
	// Initialize the rigid body
	if (rigidBody->Initialize(boxinfo.colliderInfo, shape, m_Physics))
	{
		rigidBody->SetExtent(boxinfo.Extent);

		// Insert the rigid body into the map
		m_RigidBodyMap.insert(std::make_pair(boxinfo.colliderInfo.EntityID, rigidBody));
		// Add the rigid body to the scene
		AddBodyScene(rigidBody);
		shape->release();
	}
	else
		assert(false);

}

void RigidBodyManager::CreateStaticBody(const SphereColliderInfo sphereinfo, EColliderType collidertype, const PhysicsInfo engininfo)
{
	// Create the material for the Sphere
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(sphereinfo.colliderInfo.StaticFriction, sphereinfo.colliderInfo.DynamicFriction, sphereinfo.colliderInfo.Restitution);
	// Create the shape for the sphere collider
	physx::PxShape* shape = m_Physics->createShape(physx::PxSphereGeometry(sphereinfo.Radius), *pxMaterial);
	// Set up the static body
	StaticRigidBody* rigidBody = SettingStaticBody(shape, sphereinfo.colliderInfo, collidertype, engininfo);
	// Create collision data
	//CollisionData* collisiondata = new CollisionData;
	// Initialize the rigid body
	if (rigidBody->Initialize(sphereinfo.colliderInfo, shape, m_Physics))
	{
		rigidBody->SetRadius(sphereinfo.Radius);
		// Insert the rigid body into the map
		m_RigidBodyMap.insert(std::make_pair(sphereinfo.colliderInfo.EntityID, rigidBody));
		// Add the rigid body to the scene
		AddBodyScene(rigidBody);
		shape->release();
	}
	else
		assert(false);
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
	//CollisionData* collisiondata = new CollisionData;
	// Initialize the rigid body
	if (rigidBody->Initialize(capsuleinfo.colliderInfo, shape, m_Physics))
	{
		// Insert the rigid body into the map
		m_RigidBodyMap.insert(std::make_pair(capsuleinfo.colliderInfo.EntityID, rigidBody));
		// Add the rigid body to the scene
		AddBodyScene(rigidBody);
		shape->release();
	}
	else
		assert(false);


}

void RigidBodyManager::CreateDynamicBody(const VPPhysics::BoxColliderInfo boxinfo, EColliderType collidertype, const VPPhysics::PhysicsInfo engininfo)
{
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(boxinfo.colliderInfo.StaticFriction, boxinfo.colliderInfo.DynamicFriction, boxinfo.colliderInfo.Restitution);
	physx::PxShape* shape = m_Physics->createShape(physx::PxBoxGeometry(boxinfo.Extent.x, boxinfo.Extent.y, boxinfo.Extent.z), *pxMaterial);
	DynamicRigidBody* rigidBody = SettingDynamicBody(shape, boxinfo.colliderInfo, collidertype, engininfo);
	//CollisionData* collisiondata = new CollisionData;
	if (rigidBody->Initialize(boxinfo.colliderInfo, shape, m_Physics))
	{
		m_RigidBodyMap.insert(std::make_pair(boxinfo.colliderInfo.EntityID, rigidBody));
		AddBodyScene(rigidBody);
		shape->release();
		rigidBody->SetExtent(boxinfo.Extent);
	}
	else
		assert(false);

}

void RigidBodyManager::CreateDynamicBody(const VPPhysics::SphereColliderInfo sphereinfo, EColliderType collidertype, const VPPhysics::PhysicsInfo engininfo)
{
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(sphereinfo.colliderInfo.StaticFriction, sphereinfo.colliderInfo.DynamicFriction, sphereinfo.colliderInfo.Restitution);
	physx::PxShape* shape = m_Physics->createShape(physx::PxSphereGeometry(sphereinfo.Radius), *pxMaterial);
	DynamicRigidBody* rigidBody = SettingDynamicBody(shape, sphereinfo.colliderInfo, collidertype, engininfo);
	//CollisionData* collisiondata = new CollisionData;
	if (rigidBody->Initialize(sphereinfo.colliderInfo, shape, m_Physics))
	{
		m_RigidBodyMap.insert(std::make_pair(sphereinfo.colliderInfo.EntityID, rigidBody));
		AddBodyScene(rigidBody);
		shape->release();
		rigidBody->SetRadius(sphereinfo.Radius);
	}
	else
		assert(false);
}



void RigidBodyManager::CreateDynamicBody(const VPPhysics::CapsuleColliderInfo capsuleinfo, EColliderType collidertype, const VPPhysics::PhysicsInfo engininfo)
{
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(capsuleinfo.colliderInfo.StaticFriction, capsuleinfo.colliderInfo.DynamicFriction, capsuleinfo.colliderInfo.Restitution);
	physx::PxShape* shape = m_Physics->createShape(physx::PxCapsuleGeometry(capsuleinfo.Radius,capsuleinfo.HalfHeight), *pxMaterial);
	DynamicRigidBody* rigidBody = SettingDynamicBody(shape, capsuleinfo.colliderInfo, collidertype, engininfo);
	//CollisionData* collisiondata = new CollisionData;
	if (rigidBody->Initialize(capsuleinfo.colliderInfo, shape, m_Physics))
	{
		m_RigidBodyMap.insert(std::make_pair(capsuleinfo.colliderInfo.EntityID, rigidBody));
		AddBodyScene(rigidBody);
		shape->release();
		rigidBody->SetHalfHeight(capsuleinfo.HalfHeight);
		rigidBody->SetRadius(capsuleinfo.Radius);
	}
	else
		assert(false);
}

StaticRigidBody* RigidBodyManager::SettingStaticBody(physx::PxShape* shape, const ColliderInfo& info, const EColliderType& colliderType, const PhysicsInfo engininfo)
{
	physx::PxFilterData filterdata;
	filterdata.word0 = (int)info.PhysicsLayer;
	filterdata.word1 = engininfo.CollisionMatrix[(int)info.PhysicsLayer];
	shape->setSimulationFilterData(filterdata);
	std::shared_ptr<StaticRigidBody> staticBody = std::make_shared<StaticRigidBody>(colliderType, info.EntityID, info.PhysicsLayer);


	m_RigidBodyMap.insert(std::make_pair(staticBody->GetID(), staticBody));
	return staticBody.get();
}

DynamicRigidBody* RigidBodyManager::SettingDynamicBody(physx::PxShape* shape, const ColliderInfo& info, const EColliderType& colliderType, const VPPhysics::PhysicsInfo engininfo)
{
	physx::PxFilterData filterdata;
	filterdata.word0 = (int)info.PhysicsLayer;
	filterdata.word1 = engininfo.CollisionMatrix[(int)info.PhysicsLayer];
	shape->setSimulationFilterData(filterdata);
	std::shared_ptr<DynamicRigidBody> dynamicBody = std::make_shared<DynamicRigidBody>(colliderType, info.EntityID, info.PhysicsLayer);


	m_RigidBodyMap.insert(std::make_pair(dynamicBody->GetID(), dynamicBody));
	return dynamicBody.get();
}

void RigidBodyManager::OnAddBodyScene(std::any data)
{
	auto tempRigid = std::any_cast<RigidBody*>(data);

	DynamicRigidBody* dynamicBody = dynamic_cast<DynamicRigidBody*>(tempRigid);
	if (dynamicBody)
		m_Scene->addActor(*dynamicBody->GetPxDynamicRigid());

	StaticRigidBody* StaticBody = dynamic_cast<StaticRigidBody*>(tempRigid);
	if (StaticBody)
		m_Scene->addActor(*StaticBody->GetPxStaticRigid());
}

void RigidBodyManager::OnReleaseBodyScene(std::any data)
{
	auto [rigidBody,IsDynamic] = std::any_cast<std::pair<std::shared_ptr<RigidBody>, bool>>(data);

	if (IsDynamic)
	{
		DynamicRigidBody* dynamicBody = dynamic_cast<DynamicRigidBody*>(rigidBody.get());
		m_Scene->removeActor(*dynamicBody->GetPxDynamicRigid());
	}
	else
	{
		StaticRigidBody* StaticBody = dynamic_cast<StaticRigidBody*>(rigidBody.get());
		m_Scene->removeActor(*StaticBody->GetPxStaticRigid());
	}
	// Ensure that the rigidBody is properly deleted
	//delete rigidBody;

}

void RigidBodyManager::ReleaseBodyScene(uint32_t entityID)
{
	if (m_RigidBodyMap.find(entityID) == m_RigidBodyMap.end())
		return;

	auto tempbody = m_RigidBodyMap.find(entityID)->second;
	bool IsDynamic = false;
	if (tempbody)
	{
		DynamicRigidBody* dynamicBody = dynamic_cast<DynamicRigidBody*>(tempbody.get());
		if (dynamicBody)
		{
			if (dynamicBody->GetPxDynamicRigid()->getScene() == m_Scene)
			{
				m_RigidBodyMap.erase(m_RigidBodyMap.find(entityID));
				IsDynamic = true;
			}
		}
		StaticRigidBody* staticBody = dynamic_cast<StaticRigidBody*>(tempbody.get());
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

std::shared_ptr<RigidBody> RigidBodyManager::GetRigidBody(uint32_t EntityID)
{
	if (!HasRigidBody(EntityID))
	return nullptr;
	return m_RigidBodyMap[EntityID];
}

bool RigidBodyManager::HasRigidBody(uint32_t EntityID)
{
	return m_RigidBodyMap.count(EntityID) > 0;
}

void RigidBodyManager::SetGobalPose(uint32_t entityID, VPMath::Vector3 P, VPMath::Quaternion Q)
{
	auto temp = GetRigidBody(entityID);
	if (!temp) {
		// RigidBody가 존재하지 않는 경우 처리
		return;
	}
	PxVec3 tempPos = { P.x, P.y, P.z };
	Q.Normalize(); // Normalize quaternion before setting
	physx::PxQuat tempQuat = physx::PxQuat(Q.x, Q.y, Q.z, Q.w);
	if (Reflection::GetTypeID<DynamicRigidBody>() == temp->GetTypeID())
	{
		DynamicRigidBody* dynamicbody = static_cast<DynamicRigidBody*>(temp.get());

		dynamicbody->GetPxDynamicRigid()->setGlobalPose({ tempPos ,tempQuat });
		return;
	}
	if (Reflection::GetTypeID<StaticRigidBody>() == temp->GetTypeID())
	{
		StaticRigidBody* dynamicbody = static_cast<StaticRigidBody*>(temp.get());

		dynamicbody->GetPxStaticRigid()->setGlobalPose({ tempPos ,tempQuat });
		return;
	}
	
	assert(false);
}

VPMath::Vector3 RigidBodyManager::GetVelocity(uint32_t entityID)
{
	auto temp = GetRigidBody(entityID);
	if (temp == nullptr)
		assert(false);

	if (Reflection::GetTypeID<DynamicRigidBody>() == temp->GetTypeID())
	{
		DynamicRigidBody* dynamicBody = static_cast<DynamicRigidBody*>(temp.get());
		PxVec3 velocity = dynamicBody->GetPxDynamicRigid()->getLinearVelocity();

		return { velocity.x,velocity.y,velocity.z };
	}
	if (Reflection::GetTypeID<StaticRigidBody>() == temp->GetTypeID())
	{
		StaticRigidBody* staticBody = static_cast<StaticRigidBody*>(temp.get());

		return { 0,0,0};
	}
}

void RigidBodyManager::AddVelocity(uint32_t entityID, VPMath::Vector3 dir, float V)
{
	auto temp = GetRigidBody(entityID);
	if (temp == nullptr)
	{
		assert(false);
		return;
	}

	if (Reflection::GetTypeID<DynamicRigidBody>() == temp->GetTypeID())
	{
		DynamicRigidBody* dynamicBody = static_cast<DynamicRigidBody*>(temp.get());
		dir.Normalize();
		PxVec3 force =  { dir.x,dir.y,dir.z };
		force *= V;
		dynamicBody->GetPxDynamicRigid()->addForce(force,PxForceMode::eACCELERATION);

	}
		return;

}

VPMath::Vector3 RigidBodyManager::GetGobalLocation(uint32_t entityID) 
{
	auto temp = GetRigidBody(entityID);

	if (Reflection::GetTypeID<DynamicRigidBody>() == temp->GetTypeID())
	{
		DynamicRigidBody* dynamicbody = static_cast<DynamicRigidBody*>(temp.get());
		auto pose = dynamicbody->GetPxDynamicRigid()->getGlobalPose();
		VPMath::Vector3 templocation = { pose.p.x ,pose.p.y ,pose.p.z };
		return templocation;
	}
	if (Reflection::GetTypeID<StaticRigidBody>() == temp->GetTypeID())
	{
		StaticRigidBody* staticbody = static_cast<StaticRigidBody*>(temp.get());
		auto pose = staticbody->GetPxStaticRigid()->getGlobalPose();
		VPMath::Vector3 templocation = { pose.p.x ,pose.p.y ,pose.p.z };
		return templocation;
	}
	assert(false); // Add an assert to handle the unexpected case
	return {};
}

VPMath::Quaternion RigidBodyManager::GetGobalQuaternion(uint32_t entityID) {
	auto temp = GetRigidBody(entityID);

	if (Reflection::GetTypeID<DynamicRigidBody>() == temp->GetTypeID())
	{
		DynamicRigidBody* dynamicbody = static_cast<DynamicRigidBody*>(temp.get());
		auto pose = dynamicbody->GetPxDynamicRigid()->getGlobalPose();
		VPMath::Quaternion tempQuat = { pose.q.x ,pose.q.y ,pose.q.z,pose.q.w };
		tempQuat.Normalize(); // Normalize quaternion after getting it
		return tempQuat;
	}
	if (Reflection::GetTypeID<StaticRigidBody>() == temp->GetTypeID())
	{
		StaticRigidBody* staticbody = static_cast<StaticRigidBody*>(temp.get());
		auto pose = staticbody->GetPxStaticRigid()->getGlobalPose();
		VPMath::Quaternion tempQuat = { pose.q.x ,pose.q.y ,pose.q.z,pose.q.w };
		tempQuat.Normalize(); // Normalize quaternion after getting it
		return tempQuat;
	}
	assert(false); // Add an assert to handle the unexpected case
	return {};
}
void RigidBodyManager::AddBodyScene(RigidBody* body)
{
	
	EventManager::GetInstance().ScheduleEvent("OnAddBodyScene", body);

}

