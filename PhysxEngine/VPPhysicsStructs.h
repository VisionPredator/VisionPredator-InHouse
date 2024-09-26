#pragma once
#include <bitset>

namespace VPPhysics
{
	constexpr uint32_t noneID = 0;


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
	enum class ControllerPivot
	{
		CENTER=0,
		FOOT,
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
	struct ControllerInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_VER2(ControllerInfo, LayerNumber, Pivot, LocalOffset)

		uint32_t EntityId = noneID;								// ĳ���� ��Ʈ�ѷ� ���̵�
		EPhysicsLayer LayerNumber{};								// �浹 ��Ʈ���� ���̾� �ѹ�
		ControllerPivot Pivot{};								//��Ʈ�ѷ��� Pivot ��ġ
		VPMath::Vector3 LocalOffset{ 0.f,0.f ,0.f };

	};
	struct CapsuleControllerInfo
	{

		NLOHMANN_DEFINE_TYPE_INTRUSIVE_VER2(CapsuleControllerInfo, Info, position, height, radius, stepOffset, slopeLimit, contactOffset);

		ControllerInfo Info{};
		VPMath::Vector3 position{ 0.f, 0.f, 0.f };					// ĳ���� ��Ʈ�ѷ��� ��ġ�ϴ� ó�� ���� ��ġ
		float height = 0.1f;										// ĳ���� ��Ʈ�ѷ�(ĸ��)�� ����
		float radius = 0.05f;										// ĳ���� ��Ʈ�ѷ�(ĸ��)�� ������
		float stepOffset = 0.0f;									// ĳ���� ��Ʈ�ѷ��� ������ �� �ִ� 
		float slopeLimit = 0.3f;									// ĳ���Ͱ� �ɾ� �ö� �� �ִ� �ִ� ����
		float contactOffset = 0.001f;								// ��Ʈ�ѷ��� ���� ������ : ��ġ ���е� ������ �����ϱ� ���� ����մϴ�.
	};

	struct CharacterMovementInfo
	{
		float maxSpeed = 0.025f;							// �̵� �ִ� �ӵ� : ĳ���Ͱ� ������ �� �ִ� �ִ� �ӵ�
		float acceleration = 1.f;							// ���ӵ� : ĳ���Ͱ� �Է� ���� ���� �� �̵� ���ӵ�
		float staticFriction = 0.4f;						// ���� ���� ��� : ĳ���Ͱ� �̵� �� ������ �� ĳ���Ͱ� �޴� ������ ( 0.0f ~ 1.f )
		float dynamicFriction = 0.1f;						// ���� ���� ��� : �̵� �߿� ĳ���Ͱ� �޴� ������ ( 0.0f ~ 1.f )
		float jumpSpeed = 0.05f;							// ����(y��) �ӵ�
		float jumpXZAcceleration = 10.f;					// ���� �߿� �̵�(XZ��) ���ӵ� ��
		float jumpXZDeceleration = 0.1f;					// ���� �߿� �̵�(XZ��) ���� �� ( 0.0 ~ 1.0 )
		float gravityWeight = 0.2f;							// �⺻ �߷� ���� �� �� ������ ����ġ�� �� �ְ� ���� �� ���� �ٸ��� ������ �� �ֽ��ϴ�.

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
		float StaticFriction = 1.f;							// ���� ���� ���
		float DynamicFriction = 1.f;						// ���� ���� ���
		float Restitution = 0.f;							// ���� ���
		float Density = 1.f;								// �е�
	};

	struct BoxColliderInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_VER2(BoxColliderInfo,  Extent)

		ColliderInfo colliderInfo{};
		VPMath::Vector3 Extent = {1,1,1};		// ����
	};

	struct SphereColliderInfo
	{
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_VER2(SphereColliderInfo,  Radius)

		ColliderInfo colliderInfo{};
		float Radius = 1.f;									// ������
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
		float Radius = 1.f;									// ������
		float HalfHeight = 1.f;								// ����
	};

}
