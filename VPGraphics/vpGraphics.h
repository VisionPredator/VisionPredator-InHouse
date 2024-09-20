#pragma once

// enum 및 struct 들 모아놓는 곳.
// debug, particle, decal, trail, etc..

namespace debug
{
	struct SphereInfo
	{
		DirectX::BoundingSphere Sphere;
		VPMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};

	struct AABBInfo	// AABB
	{
		DirectX::BoundingBox AABB;
		VPMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};

	struct OBBInfo
	{
		DirectX::BoundingOrientedBox OBB;
		float xAxisAngle = 0.f;
		float yAxisAngle = 0.f;
		float zAxisAngle = 0.f;
		VPMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};

	struct FrustumInfo
	{
		DirectX::BoundingFrustum Frustum;
		VPMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};

	struct GridInfo
	{
		VPMath::Vector3 XAsix;
		VPMath::Vector3 YAsix;
		VPMath::Vector3 Origin;
		size_t XDivs;
		size_t YDivs;
		float GridSize;
		VPMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};

	struct RingInfo
	{
		VPMath::Vector3 Origin;
		VPMath::Vector3 MajorAxis;
		VPMath::Vector3 MinorAxis;
		VPMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};

	struct TriangleInfo
	{
		VPMath::Vector2 PointA;
		VPMath::Vector2 PointB;
		VPMath::Vector2 PointC;
		VPMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};

	struct QuadInfo
	{
		VPMath::Vector2 PointA;
		VPMath::Vector2 PointB;
		VPMath::Vector2 PointC;
		VPMath::Vector2 PointD;
		VPMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};

	struct RayInfo
	{
		VPMath::Vector3 Origin;
		VPMath::Vector3 Direction;
		bool Normalize = true;
		VPMath::Color Color = { 1.f, 1.f, 1.f, 1.f };
	};


}

namespace effect
{
	struct ParticleInfo
	{
		std::string TexturePath;
		UINT MaxParticles = 0;
		VPMath::Vector2 Size = { 1.0f, 1.0f };
		VPMath::Vector3 Direction = VPMath::Vector3(0.0f, 1.0f, 0.0f);
	};

}

namespace ui
{
	enum class RenderModeType
	{
		WorldSpace,				// 3D 월드 좌표계에서 렌더링
		ScreenSpaceOverlay,		// 2D 화면 좌표계에서 렌더링
		ScreenSpaceCamera,		// 특정 카메라에 연결하여 렌더링..		// 유니티를 참고함
	};

	struct ImageInfo
	{
		std::string ImagePath = {};
		VPMath::Color Color = { 1.f, 1.f, 1.f, 1.f };	// 이미지 색상. rgba
		RenderModeType RenderMode = RenderModeType::WorldSpace;

		// 2D info
		float PosXPercent = 0.f;
		float PosYPercent = 0.f;
		float Scale = 1.f;		
		uint32_t Layer = 0;		// 이미지 깊이값

		// 3D info
		VPMath::Matrix World = VPMath::Matrix::Identity;
		VPMath::Vector3 Rotation = VPMath::Vector3::Zero;
	};

	struct TextInfo
	{
		std::wstring Text = {};
		std::wstring FontPath = {};
		VPMath::Color Color;
	};
}