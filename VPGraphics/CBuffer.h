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
	LightData() : ambient(0,0,0,0), diffuse(),specular(),direction(),range(-1),attenuation(),pad(), pos(0, 0, 0), spot(-1)
	{

	}


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


struct LightArray
{
	LightArray() : Dir(),Point(),Spot(),DirIndex(0),PointIndex(0), SpotIndex(0),pad(0)
	{

	}

	//여유있게 대충 100개 했음
	LightData Dir[100];
	LightData Point[100];
	LightData Spot[100];
	float DirIndex;
	float PointIndex;
	float SpotIndex;
	float pad;
};