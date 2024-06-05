#pragma once
#include "../include/directxtk/SimpleMath.h"


struct TransformData
{
	TransformData() : world(DirectX::SimpleMath::Matrix::Identity),local(DirectX::SimpleMath::Matrix::Identity)
	{
	}

	DirectX::SimpleMath::Matrix world;
	DirectX::SimpleMath::Matrix local;
};

struct CameraData
{
	DirectX::SimpleMath::Matrix worldviewproj;
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix proj;
	DirectX::SimpleMath::Matrix viewInverse;
};

struct MatrixPallete
{
	DirectX::SimpleMath::Matrix pallete[128];
};


enum class Kind_of_Light
{
	Direction = 0,
	Spot,
	Point,

	End
};

struct LightData
{
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;

	DirectX::XMFLOAT3 direction;
	float range;

	DirectX::XMFLOAT3 attenuation;
	float pad;

	DirectX::XMFLOAT3 pos;
	float spot;


};