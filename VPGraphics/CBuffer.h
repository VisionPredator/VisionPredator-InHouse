#pragma once
#include "../include/directxtk/SimpleMath.h"


struct TransformData
{
	TransformData() : world(DirectX::SimpleMath::Matrix::Identity),local(DirectX::SimpleMath::Matrix::Identity),
		localInverse(DirectX::SimpleMath::Matrix::Identity), worldInverse(DirectX::SimpleMath::Matrix::Identity)
	{
	}

	DirectX::SimpleMath::Matrix world;
	DirectX::SimpleMath::Matrix local;

	DirectX::SimpleMath::Matrix localInverse;
	DirectX::SimpleMath::Matrix worldInverse;
};

struct CameraData
{
	DirectX::SimpleMath::Matrix worldviewproj;
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix proj;
	DirectX::SimpleMath::Matrix viewInverse;
	DirectX::SimpleMath::Matrix projInverse;
};

struct MatrixPallete
{
	DirectX::SimpleMath::Matrix offset[128];
};


enum class LightType
{
	Direction = 0,
	Spot,
	Point,

	End
};

struct LightData
{
	LightData() : direction(0,0,0), range(-1), attenuation(0,0,0), type(-1), pos(0, 0, 0), spot(-1), color(1, 1, 1), intensity(1)
	{

	}

	DirectX::XMFLOAT3 direction;
	float range;

	DirectX::XMFLOAT3 attenuation;
	float type;

	DirectX::XMFLOAT3 pos;
	float spot;

	DirectX::XMFLOAT3 color;
	float intensity;
};


struct LightArray
{
	LightArray() : DirIndex(0),PointIndex(0), SpotIndex(0),pad(0)
	{
	}


	//여유있게 대충 100개 했음
	LightData array[100];
	float DirIndex;
	float PointIndex;
	float SpotIndex;
	float pad;
};

struct MaterialData
{
	MaterialData() : albedo(1,1,1,1), metalness(0),roughness(0),ao(0),useNE(0,0,0,0), useAMRO(0,0,0,0)
	{

	}

	DirectX::XMFLOAT4 useAMRO;	//텍스처를 가지고 있는가 albedo, metalic, roughness, AO
	DirectX::XMFLOAT4 useNE;	//텍스처를 가지고 있는가 normal, Emissive
	DirectX::XMFLOAT4 albedo;
	float metalness;
	float roughness;
	float ao; // Ambient Occlusion
	float pad;
};