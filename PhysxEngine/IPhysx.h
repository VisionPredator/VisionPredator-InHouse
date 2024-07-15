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
	virtual const VPPhysics::PhysicsInfo GetPhysicEngineInfo()abstract;
	virtual void  SetPhysicEngineInfo(VPPhysics::PhysicsInfo engineinfo) abstract;
	virtual void SetGobalPose(uint32_t entityID, VPMath::Vector3 P, VPMath::Quaternion Q) abstract;
	virtual VPMath::Vector3 GetGobalLocation(uint32_t entityID) abstract;
	virtual VPMath::Quaternion GetGobalQuaternion(uint32_t entityID) abstract;

};

}
