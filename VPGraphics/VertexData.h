#pragma once
#include <d3d11.h>
#include "CBuffer.h"

struct BaseVertex
{
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT4 normal;
	DirectX::XMFLOAT4 tangent;
	DirectX::XMFLOAT4 bitangent;
	DirectX::XMFLOAT2 TexCord;
	DirectX::XMFLOAT2 LightMapUV;
};

struct SkinningVertex
{
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT4 normal;
	DirectX::XMFLOAT4 tangent;
	DirectX::XMFLOAT4 bitangent;
	DirectX::XMFLOAT2 TexCord;
	DirectX::XMFLOAT2 LightMapUV;
	float BoneIndices[8];
	float BoneWeights[8];
};

struct QuadVertex
{
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT2 TexCord;
};

//¿ŒΩ∫≈œΩÃ
struct InstanceData
{
	InstanceData() : world(VPMath::Matrix::Identity),worldInverse(VPMath::Matrix::Identity)
	{

	}
	/*
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT4 normal;
	DirectX::XMFLOAT4 tangent;
	DirectX::XMFLOAT4 bitangent;
	DirectX::XMFLOAT2 TexCord;
	DirectX::XMFLOAT2 LightMapUV;
	*/

	VPMath::Matrix world;
	VPMath::Matrix worldInverse;
	VPMath::XMFLOAT2 lightmap_offset;
	VPMath::XMFLOAT2 lightmap_tiling;
	VPMath::XMFLOAT2 lightmap_index;
};


struct InstanceSkinnedData
{
	InstanceSkinnedData() : world(VPMath::Matrix::Identity), worldInverse(VPMath::Matrix::Identity)
	{

	}

	VPMath::Matrix world;
	VPMath::Matrix worldInverse;
};



struct DecalVertex
{
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 normal;
	DirectX::XMFLOAT4 tex;
};

//¿ŒΩ∫≈œΩÃ
struct InstanceDecalData
{
	InstanceDecalData() : world(VPMath::Matrix::Identity), worldInverse(VPMath::Matrix::Identity)
	{

	}

	VPMath::Matrix world;
	VPMath::Matrix worldInverse;
};
