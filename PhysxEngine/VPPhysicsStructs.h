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
}
