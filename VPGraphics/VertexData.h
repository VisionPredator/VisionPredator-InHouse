#pragma once
#include <d3d11.h>

#pragma region BaseVertex
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

const UINT TextureDescCount = 7;

static D3D11_INPUT_ELEMENT_DESC TextureVertexInputDesc[] = {
	{"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"NORMAL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,32,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"TANGENT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,48,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"BITANGENT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,64,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,80,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"LIGHTMAPUV",0,DXGI_FORMAT_R32G32_FLOAT,0,88,D3D11_INPUT_PER_VERTEX_DATA,0}
};
#pragma endregion BaseVertex

#pragma region SkinningVertex
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

const UINT SkinningDescCount = 11;

static D3D11_INPUT_ELEMENT_DESC SkinningVertexInputDesc[] = {
	{"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"NORMAL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,32,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"TANGENT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,48,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"BITANGENT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,64,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,80,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"LIGHTMAPUV",0,DXGI_FORMAT_R32G32_FLOAT,0,88,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"BONEINDEX",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,96,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"BONEINDEX",1,DXGI_FORMAT_R32G32B32A32_FLOAT,0,112,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"BONEWEIGHT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,128,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"BONEWEIGHT",1,DXGI_FORMAT_R32G32B32A32_FLOAT,0,144,D3D11_INPUT_PER_VERTEX_DATA,0},
};
#pragma endregion SkinningVertex


#pragma region QuadVertex
struct QuadVertex
{
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT2 TexCord;
};

const UINT QuadDescCount = 2;

static D3D11_INPUT_ELEMENT_DESC QuadVertexInputDesc[] = {
	{"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0}
};
#pragma endregion QuadVertex