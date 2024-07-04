#pragma once

namespace VPPhysics
{

	/// <summary>
	/// TRIGGER: �浹���� �ʰ� ��ġ�� ������ �̺�Ʈ�� ȣ��.
	/// COLLISION: �浹�ǰ� �ݸ��� �̺�Ʈ�� ȣ��.
	/// </summary>
	enum class EColliderType
	{
		TRIGGER,
		COLLISION
	};

	/// <summary>
	/// �ݸ��� : �浹 ������
	/// </summary>
	struct CollisionData
	{
		uint32_t myId;
		uint32_t otherId;
		uint32_t myLayerNumber;
		unsigned int otherLayerNumber;
		std::vector<VPMath::Vector3> ContectPoints;
		bool isDead = false;
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
	constexpr uint32_t noneID = 0;

	struct ColliderInfo
	{
		uint32_t LayerNumber = 0;
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
		ColliderInfo colliderInfo;
		DirectX::SimpleMath::Vector3 Extent = {};		// ����
	};

	struct SphereColliderInfo
	{
		ColliderInfo colliderInfo;
		float raidus = 1.f;									// ������
	};

	struct CapsuleColliderInfo
	{
		ColliderInfo colliderInfo;
		float raidus = 1.f;									// ������
		float halfHeight = 1.f;								// ����
	};
}
