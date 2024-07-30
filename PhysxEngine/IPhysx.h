#pragma once
#include "VPPhysicsStructs.h"
using namespace VPPhysics;
namespace Physic
{
class IPhysx
{
public:
	IPhysx() {};
	virtual ~IPhysx() {};

	virtual bool Initialize()  abstract;
	virtual void Update(float deltatime) abstract;
	virtual bool Finalize()  abstract;
	virtual void CreateStaticBody(const VPPhysics::BoxColliderInfo boxinfo, EColliderType collidertype) abstract;
	virtual void CreateStaticBody(const VPPhysics::SphereColliderInfo sphereinfo, EColliderType collidertype) abstract;
	virtual void CreateStaticBody(const VPPhysics::CapsuleColliderInfo capsuleinfo, EColliderType collidertype) abstract;
	virtual void CreateDynamicBody(const VPPhysics::BoxColliderInfo boxinfo, EColliderType collidertype) abstract;
	virtual void CreateDynamicBody(const VPPhysics::SphereColliderInfo sphereinfo, EColliderType collidertype) abstract;
	virtual void CreateDynamicBody(const VPPhysics::CapsuleColliderInfo capsuleinfo, EColliderType collidertype) abstract;

	virtual void ReleaseActor(uint32_t entityID) abstract;
	virtual const VPPhysics::PhysicsInfo GetPhysicsInfo()abstract;
	virtual void  SetPhysicsInfo(VPPhysics::PhysicsInfo engineinfo) abstract;
	virtual void  ApplyPhysicEngineInfo() abstract;
	virtual void SetGobalPose(uint32_t entityID, VPMath::Vector3 P, VPMath::Quaternion Q) abstract;
	virtual VPMath::Vector3 GetGobalLocation(uint32_t entityID) abstract;
	virtual VPMath::Quaternion GetGobalQuaternion(uint32_t entityID) abstract;

	virtual void CreatCapsuleController(VPPhysics::CapsuleControllerInfo capsuleinfo) abstract;
	virtual void RemoveController(uint32_t entityID) abstract;
	virtual void SetControllerGobalPose(uint32_t entityID, VPMath::Vector3 P) abstract;
	virtual void SetControllerVelocity(uint32_t entityID,VPMath::Vector3 velocity) abstract;
	virtual VPMath::Vector3 GetControllerGobalPose(uint32_t entityID) abstract;
	virtual bool GetControllerIsFall(uint32_t entityID) abstract;

	virtual void AddVelocity(uint32_t entityID, VPMath::Vector3 Dir, float velocity) abstract;
	virtual VPMath::Vector3 GetVelocity(uint32_t entityID)abstract;
	virtual void AddImpulse(uint32_t entityID, VPMath::Vector3 Dir, float power) abstract;
	virtual void LoadConvexMeshResource(const VPPhysics::ConvexMeshInfo& info) abstract;


	virtual bool HasConvexMeshResource(const std::string& key) abstract;



};

}
