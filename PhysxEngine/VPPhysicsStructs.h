#pragma once

namespace VPPhysics
{

	/// <summary>
	/// TRIGGER: 충돌되지 않고 겹치면 오버랩 이벤트만 호출.
	/// COLLISION: 충돌되고 콜리전 이벤트도 호출.
	/// </summary>
	enum class EColliderType
	{
		TRIGGER,
		COLLISION
	};

	/// <summary>
	/// 콜리전 : 충돌 데이터
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
}
