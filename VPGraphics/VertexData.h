#pragma once
#include <d3d11.h>

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
