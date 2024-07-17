#pragma once
#include <bitset>

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
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(PhysicsInfo, Gravity, FrameRate, CollisionMatrix)
			void SerializePhysicsInfo(nlohmann::json& json)
		{
			to_json(json, *this);
		}

		VPMath::Vector3 Gravity{};
		int FrameRate = 60;
		std::array<int, (int)EPhysicsLayer::END> CollisionMatrix{ };
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
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ColliderInfo, PhysicsLayer, UseGravity, LinearLock, AngleLock, OffSet, StaticFriction, DynamicFriction, Restitution, Density)

		EPhysicsLayer PhysicsLayer{};
		uint32_t EntityID = noneID;
		VPMath::Vector3	WorldLocation = {};
		VPMath::Quaternion	WorldQuaternion = {};
		bool UseGravity{};
		std::array<bool, 3> LinearLock{};
		std::array<bool, 3> AngleLock{};
		VPMath::Vector3 OffSet = {};
		float StaticFriction = 1.f;							// 정적 마찰 계수
		float DynamicFriction = 1.f;						// 동적 마찰 계수
		float Restitution = 1.f;							// 복원 계수
		float Density = 1.f;								// 밀도
	};

	struct BoxColliderInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(BoxColliderInfo, UseAABB, Extent)

		ColliderInfo colliderInfo{};
		bool UseAABB=false;
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
