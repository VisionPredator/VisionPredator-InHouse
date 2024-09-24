#pragma once
#include <bitset>

namespace VPPhysics
{
	constexpr uint32_t noneID = 0;


	/// <summary>
	/// TRIGGER: 충돌되지 않고 겹치면 오버랩 이벤트만 호출.
	/// COLLISION: 충돌되고 콜리전 이벤트도 호출.
	/// </summary>
	enum class EColliderType
	{
		TRIGGER=0,
		COLLISION
	};
	enum class EPhysicsLayer
	{
		GROUND=0,
		WALL,
		TOP,
		PLAYER,
		OBJECT,
		ENEMY,
		DOOR,
		ACTIVEDOOR,
		TRIGGER,
		END
	};
	enum class ControllerPivot
	{
		CENTER=0,
		FOOT,
		END
	};
	/// <summary>
/// 콜백 함수에 전달되는 콜리전 이벤트 형태
/// </summary>
	enum class ECollisionEventType
	{
		ENTER_OVERLAP = 0,
		ON_OVERLAP,
		END_OVERLAP,
		ENTER_COLLISION,
		ON_COLLISION,
		END_COLLISION,
	};
	enum class EColliderShape
	{
		BOX=0,
		SPHERE,
		CAPSULE,
		CONVEX,
		END
	};
	struct PhysicsInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_VER2(PhysicsInfo, Gravity, FrameRate, CollisionMatrix)


		VPMath::Vector3 Gravity{};
		uint32_t FrameRate = 60;
		std::array<int, (int)EPhysicsLayer::END> CollisionMatrix{ };
	};
	struct ConvexMeshResourceInfo
	{
		std::wstring FBXName{};
		std::vector<VPMath::Vector3> Vertexs{};
	};
	/// <summary>
	/// 콜리전 : 충돌 데이터
	/// </summary>
	struct CollisionData
	{
		uint32_t myId;
		uint32_t otherId;
		EPhysicsLayer myLayerNumber;
		unsigned int otherLayerNumber;
		std::vector<VPMath::Vector3> ContectPoints;
		bool isDead = false;
	};
	struct ControllerInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_VER2(ControllerInfo, LayerNumber, Pivot, LocalOffset)

		uint32_t EntityId = noneID;								// 캐릭터 컨트롤러 아이디
		EPhysicsLayer LayerNumber{};								// 충돌 매트릭스 레이어 넘버
		ControllerPivot Pivot{};								//컨트롤러의 Pivot 위치
		VPMath::Vector3 LocalOffset{ 0.f,0.f ,0.f };

	};
	struct CapsuleControllerInfo
	{

		NLOHMANN_DEFINE_TYPE_INTRUSIVE_VER2(CapsuleControllerInfo, Info, position, height, radius, stepOffset, slopeLimit, contactOffset);

		ControllerInfo Info{};
		VPMath::Vector3 position{ 0.f, 0.f, 0.f };					// 캐릭터 컨트롤러가 위치하는 처음 생성 위치
		float height = 0.1f;										// 캐릭터 컨트롤러(캡슐)의 높이
		float radius = 0.05f;										// 캐릭터 컨트롤러(캡슐)의 반지름
		float stepOffset = 0.0f;									// 캐릭터 컨트롤러가 지나갈 수 있는 
		float slopeLimit = 0.3f;									// 캐릭터가 걸어 올라갈 수 있는 최대 기울기
		float contactOffset = 0.001f;								// 컨트롤러의 접촉 오프셋 : 수치 정밀도 문제를 방지하기 위해 사용합니다.
	};

	struct CharacterMovementInfo
	{
		float maxSpeed = 0.025f;							// 이동 최대 속도 : 캐릭터가 움직일 수 있는 최대 속도
		float acceleration = 1.f;							// 가속도 : 캐릭터가 입력 값을 받을 때 이동 가속도
		float staticFriction = 0.4f;						// 정적 마찰 계수 : 캐릭터가 이동 중 멈췄을 때 캐릭터가 받는 마찰력 ( 0.0f ~ 1.f )
		float dynamicFriction = 0.1f;						// 동적 마찰 계수 : 이동 중에 캐릭터가 받는 마찰력 ( 0.0f ~ 1.f )
		float jumpSpeed = 0.05f;							// 점프(y축) 속도
		float jumpXZAcceleration = 10.f;					// 점프 중에 이동(XZ축) 가속도 값
		float jumpXZDeceleration = 0.1f;					// 점프 중에 이동(XZ축) 감속 값 ( 0.0 ~ 1.0 )
		float gravityWeight = 0.2f;							// 기본 중력 값을 줄 수 있지만 가중치를 더 주고 싶을 때 값을 다르게 세팅할 수 있습니다.

	};



	struct ColliderInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_VER2(ColliderInfo, PhysicsLayer, UseGravity, LinearLock, AngleLock, OffSet, StaticFriction, DynamicFriction, Restitution, Density)

		EPhysicsLayer PhysicsLayer{};
		uint32_t EntityID = noneID;
		VPMath::Vector3	WorldLocation = {};
		VPMath::Quaternion	WorldQuaternion = {};
		VPMath::Vector3 WorldScale{};
		bool UseGravity{};
		std::array<bool, 3> LinearLock{};
		std::array<bool, 3> AngleLock{};
		VPMath::Vector3 OffSet = {};
		float StaticFriction = 1.f;							// 정적 마찰 계수
		float DynamicFriction = 1.f;						// 동적 마찰 계수
		float Restitution = 0.f;							// 복원 계수
		float Density = 1.f;								// 밀도
	};

	struct BoxColliderInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_VER2(BoxColliderInfo,  Extent)

		ColliderInfo colliderInfo{};
		VPMath::Vector3 Extent = {1,1,1};		// 길이
	};

	struct SphereColliderInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_VER2(SphereColliderInfo,  Radius)

		ColliderInfo colliderInfo{};
		float Radius = 1.f;									// 반지름
	};
	struct ConvexColliderInfo
	{
		ColliderInfo colliderInfo{};
		std::wstring FBXName{};
		//std::vector<VPMath::Vector3> Vertexs{};
	};
	struct CapsuleColliderInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_VER2(CapsuleColliderInfo,  Radius, HalfHeight)

		ColliderInfo colliderInfo{};
		float Radius = 1.f;									// 반지름
		float HalfHeight = 1.f;								// 높이
	};

}
