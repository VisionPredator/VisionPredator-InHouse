#pragma once

namespace VPPhysics
{

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
		END
	};
	struct PhysicsInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(PhysicsInfo, Gravity, CollisionMatrix)

		VPMath::Vector3 Gravity{};
		std::array<int, (int)EPhysicsLayer::END> CollisionMatrix{ INT_MAX ,INT_MAX ,INT_MAX ,INT_MAX ,INT_MAX ,INT_MAX };
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


	constexpr uint32_t noneID = 0;

	struct ColliderInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ColliderInfo, PhysicsLayer, StaticFriction, DynamicFriction, Restitution, Density)

		EPhysicsLayer PhysicsLayer{};
		uint32_t EntityID = noneID;
		VPMath::Vector3	WorldLocation = {};
		VPMath::Quaternion	WorldQuaternion = {};
		float StaticFriction = 1.f;							// 정적 마찰 계수
		float DynamicFriction = 1.f;						// 동적 마찰 계수
		float Restitution = 1.f;							// 복원 계수
		float Density = 1.f;								// 밀도
	};

	struct BoxColliderInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(BoxColliderInfo,  Extent)

		ColliderInfo colliderInfo{};
		VPMath::Vector3 Extent = {1,1,1};		// 길이
	};

	struct SphereColliderInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(SphereColliderInfo,  Radius)

		ColliderInfo colliderInfo{};
		float Radius = 1.f;									// 반지름
	};

	struct CapsuleColliderInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CapsuleColliderInfo,  Radius, HalfHeight)

		ColliderInfo colliderInfo{};
		float Radius = 1.f;									// 반지름
		float HalfHeight = 1.f;								// 높이
	};

}
