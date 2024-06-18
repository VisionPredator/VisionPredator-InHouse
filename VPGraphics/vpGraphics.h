#pragma once

// enum 및 struct 들 모아놓는 곳.

namespace debug
{
	struct SphereInfo
	{
		DirectX::BoundingSphere Sphere;
		DirectX::SimpleMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};

	struct BoxInfo	// AABB
	{
		DirectX::BoundingBox Box;
		DirectX::SimpleMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};

	struct OBBInfo
	{
		DirectX::BoundingOrientedBox OBB;
		DirectX::SimpleMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};

	struct FrustumInfo
	{
		DirectX::BoundingFrustum Frustum;
		DirectX::SimpleMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};

	struct GridInfo
	{
		DirectX::SimpleMath::Vector3 XAsix;
		DirectX::SimpleMath::Vector3 YAsix;
		DirectX::SimpleMath::Vector3 Origin;
		size_t XDivs;
		size_t YDivs;
		DirectX::SimpleMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};

	struct RingInfo
	{
		DirectX::SimpleMath::Vector3 Origin;
		DirectX::SimpleMath::Vector3 MajorAxis;
		DirectX::SimpleMath::Vector3 MinorAxis;
		DirectX::SimpleMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};

	struct TriangleInfo
	{
		DirectX::SimpleMath::Vector2 PointA;
		DirectX::SimpleMath::Vector2 PointB;
		DirectX::SimpleMath::Vector2 PointC;
		DirectX::SimpleMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};

	struct QuadInfo
	{
		DirectX::SimpleMath::Vector2 PointA;
		DirectX::SimpleMath::Vector2 PointB;
		DirectX::SimpleMath::Vector2 PointC;
		DirectX::SimpleMath::Vector2 PointD;
		DirectX::SimpleMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};

	struct RayInfo
	{
		DirectX::SimpleMath::Vector3 Origin;
		DirectX::SimpleMath::Vector3 Direction;
		bool Normalize = true;
		DirectX::SimpleMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};


}

