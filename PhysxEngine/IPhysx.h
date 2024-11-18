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
	virtual void CreateStaticBody(const VPPhysics::BoxColliderInfo& boxinfo, const EColliderType& collidertype) abstract;
	virtual void CreateStaticBody(const VPPhysics::SphereColliderInfo& sphereinfo, const EColliderType& collidertype) abstract;
	virtual void CreateStaticBody(const VPPhysics::CapsuleColliderInfo& capsuleinfo, const EColliderType& collidertype) abstract;
	virtual void CreateStaticBody(const VPPhysics::ConvexColliderInfo& convexinfo, const EColliderType& collidertype) abstract;
	virtual void CreateDynamicBody(const VPPhysics::BoxColliderInfo& boxinfo, const EColliderType& collidertype) abstract;
	virtual void CreateDynamicBody(const VPPhysics::SphereColliderInfo& sphereinfo, const EColliderType& collidertype) abstract;
	virtual void CreateDynamicBody(const VPPhysics::CapsuleColliderInfo& capsuleinfo, const EColliderType& collidertype) abstract;
	virtual void ConvertToStatic(uint32_t entityID) abstract;
	virtual void ConvertToDynamic(uint32_t entityID) abstract;
	virtual void ConvertToStaticWithLayer(uint32_t entityID, EPhysicsLayer layer) abstract;
	virtual void ConvertToDynamicWithLayer(uint32_t entityID, EPhysicsLayer layer) abstract;
	virtual void CreateDynamicBody(const VPPhysics::ConvexColliderInfo& convexinfo, const EColliderType& collidertype) abstract;
	virtual void ExtractVerticesAndFacesByLayer(EPhysicsLayer layer, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices) abstract;
	virtual void ExtractVerticesAndFaces(uint32_t entityID, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices) abstract;
	virtual std::vector<VPMath::Vector3> GetConVexMeshVertex(uint32_t entityID) abstract;
#pragma region raycast
	/// <summary>
	/// Actor를 가진 entity로부터 dir 방향으로 dist거리만큼 raycast를 쏴 가장가까운 entityID를 반환한다.
	/// </summary>
	/// <param name="entityID"> : Controller 또는 Rigidbody가 있는 Entity </param> 
	/// <param name="dir"> : racast 방향</param>
	/// <param name="distance"> racast 거리</param>
	/// <returns></returns>
	virtual RaycastData RaycastActor(uint32_t entityID, VPMath::Vector3 dir, float distance)abstract;
	/// <summary>
	/// Actor를 가진 entity로부터 Offset을 적용시킨 뒤 dir 방향으로 dist거리만큼 raycast를 쏴 가장 가까운 EntityID를 반환한다.
	/// </summary>
	/// <param name="entityID"> : Controller 또는 Rigidbody가 있는 Entity</param>
	/// <param name="offset"> : Entity로부터의 offset </param>
	/// <param name="dir"> : racast 방향</param>
	/// <param name="distance"> racast 거리</param>
	/// <returns></returns>
	virtual RaycastData RaycastActor_Offset(uint32_t entityID, VPMath::Vector3 offset, VPMath::Vector3 dir, float distance)abstract;
	/// <summary>
	/// location로부터 dir방향으로 dist거리만큼 raycast를 쏴 가장 가까운 EntityID를 반환한다.
	/// </summary>
	/// <param name="location"> : Raycast 시작점</param>
	/// <param name="dir"> : racast 방향</param>
	/// <param name="distance"> racast 거리</param>
	/// <returns></returns>
	virtual RaycastData RaycastActorAtPose(VPMath::Vector3 location, VPMath::Vector3 dir, float distance)abstract;
	/// <summary>
	/// location로부터 dir방향으로 dist거리만큼  raycast를 쏴 해당 entityid를 제외한 가장 가까운 EntityID를 반환한다.
	/// </summary>
	/// <param name="entityID"> : 무시할 Controller 또는 Rigidbody가 있는 Entity</param>
	/// <param name="location"> : Raycast 시작점</param>
	/// <param name="dir"> : racast 방향</param>
	/// <param name="distance"> racast 거리</param>
	/// <returns></returns>
	virtual RaycastData RaycastActorAtPose_Ignore(uint32_t entityID, VPMath::Vector3 location, VPMath::Vector3 dir, float distance) abstract;
	virtual RaycastData RaycastActorAtPose_Ignores(std::vector<uint32_t> entityIDs, VPMath::Vector3 location, VPMath::Vector3 dir, float distance) abstract;
	virtual std::vector<RaycastData> RaycastActorsAtPose(VPMath::Vector3 location, VPMath::Vector3 dir, float distance) abstract;
	virtual std::vector<RaycastData> RaycastActorsAtPose_Ignores(std::vector<uint32_t> entityIDs, VPMath::Vector3 location, VPMath::Vector3 dir, float distance) abstract;
#pragma endregion
	virtual bool IsDynamic(uint32_t entityID) abstract;
	virtual bool IsStatic(uint32_t entityID) abstract;

	virtual void ReleaseActor(uint32_t entityID) abstract;
	virtual const VPPhysics::PhysicsInfo GetPhysicsInfo()abstract;
	virtual void  SetPhysicsInfo(VPPhysics::PhysicsInfo engineinfo) abstract;
	virtual void  ApplyPhysicEngineInfo() abstract;
	virtual void SetGobalPose(uint32_t entityID, VPMath::Vector3 P, VPMath::Quaternion Q) abstract;
	virtual VPMath::Vector3 GetGobalLocation(uint32_t entityID) abstract;
	virtual VPMath::Quaternion GetGobalQuaternion(uint32_t entityID) abstract;

	virtual void CreatCapsuleController(VPPhysics::CapsuleControllerInfo capsuleinfo) abstract;
	virtual void ResizeCapsuleController(uint32_t entityID, float radius, float height) abstract;
	virtual void RemoveController(uint32_t entityID) abstract;
	virtual void SetControllerGobalPose(uint32_t entityID, VPMath::Vector3 P) abstract;
	virtual void SetControllerVelocity(uint32_t entityID, VPMath::Vector3 velocity) abstract;
	virtual VPMath::Vector3 GetControllerGobalPose(uint32_t entityID) abstract;
	virtual bool GetControllerIsFall(uint32_t entityID) abstract;

	virtual void AddVelocity(uint32_t entityID, VPMath::Vector3 Dir, float velocity) abstract;
	virtual void SetVelocity(uint32_t entityID, VPMath::Vector3 Dir = {}, float velocity = 0) abstract;
	virtual VPMath::Vector3 GetVelocity(uint32_t entityID)abstract;
	virtual void AddImpulse(uint32_t entityID, VPMath::Vector3 Dir, float power) abstract;
	virtual void LoadConvexMeshResource(const VPPhysics::ConvexMeshResourceInfo& info) abstract;
	virtual bool HasRigidBody(uint32_t entityID) abstract;
	virtual bool HasController(uint32_t entityID) abstract;
	virtual bool HasConvexMeshResource(const std::wstring& key) abstract;

};

}
