#pragma once

namespace VPPhysics
{

	/// <summary>
	/// TRIGGER: �浹���� �ʰ� ��ġ�� ������ �̺�Ʈ�� ȣ��.
	/// COLLISION: �浹�ǰ� �ݸ��� �̺�Ʈ�� ȣ��.
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
/// �ݹ� �Լ��� ���޵Ǵ� �ݸ��� �̺�Ʈ ����
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
	/// �ݸ��� : �浹 ������
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
		float StaticFriction = 1.f;							// ���� ���� ���
		float DynamicFriction = 1.f;						// ���� ���� ���
		float Restitution = 1.f;							// ���� ���
		float Density = 1.f;								// �е�
	};

	struct BoxColliderInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(BoxColliderInfo,  Extent)

		ColliderInfo colliderInfo{};
		VPMath::Vector3 Extent = {1,1,1};		// ����
	};

	struct SphereColliderInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(SphereColliderInfo,  Radius)

		ColliderInfo colliderInfo{};
		float Radius = 1.f;									// ������
	};

	struct CapsuleColliderInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CapsuleColliderInfo,  Radius, HalfHeight)

		ColliderInfo colliderInfo{};
		float Radius = 1.f;									// ������
		float HalfHeight = 1.f;								// ����
	};

}
