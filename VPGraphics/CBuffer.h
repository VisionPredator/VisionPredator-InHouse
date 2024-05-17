#pragma once
#pragma once
#include "../Include/directxtk/SimpleMath.h"


struct CB
{

};

struct WorldTransformCB : public CB
{
	DirectX::SimpleMath::Matrix world;
};

struct LocalTransformCB : public CB
{
	LocalTransformCB() : local(DirectX::SimpleMath::Matrix::Identity)
	{
	}

	DirectX::SimpleMath::Matrix local;
};

struct CameraCB : public CB
{
	DirectX::SimpleMath::Matrix worldviewproj;
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix proj;
	DirectX::SimpleMath::Matrix viewInverse;
};

struct MatrixPallete : public CB
{
	DirectX::SimpleMath::Matrix pallete[128];
};

struct DirectionLightCB : public CB
{
	DirectX::XMFLOAT4 m_ambient;
	DirectX::XMFLOAT4 m_diffuse;
	DirectX::XMFLOAT4 m_specular;
	DirectX::XMFLOAT3 m_direction;
	float pad;
};




struct PointLightCB : public CB
{
	DirectX::XMFLOAT4 m_ambient;
	DirectX::XMFLOAT4 m_diffuse;
	DirectX::XMFLOAT4 m_specular;
	DirectX::XMFLOAT3 m_attenuation;
	float m_range;
};

struct SpotLightCB : public CB
{
	DirectX::XMFLOAT4 m_ambient;
	DirectX::XMFLOAT4 m_diffuse;
	DirectX::XMFLOAT4 m_specular;

	DirectX::XMFLOAT3 m_direction;
	float m_range;

	DirectX::XMFLOAT3 m_attenuation;
	float pad;
};