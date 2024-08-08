#pragma once


struct TransformData
{
	TransformData() : world(VPMath::Matrix::Identity),local(VPMath::Matrix::Identity),
		localInverse(VPMath::Matrix::Identity), worldInverse(VPMath::Matrix::Identity)
	{
	}

	VPMath::Matrix world;
	VPMath::Matrix local;

	VPMath::Matrix localInverse;
	VPMath::Matrix worldInverse;
};

struct CameraData
{
	VPMath::Matrix worldviewproj;
	VPMath::Matrix view;
	VPMath::Matrix proj;
	VPMath::Matrix viewInverse;
	VPMath::Matrix projInverse;
	VPMath::Matrix orthoProj;	// �̷������� �����͸� �ְ� ���� �ʾҴµ�..
};

static_assert(sizeof(CameraData) % 16 == 0, "must be align");

struct MatrixPallete
{
	//�ӽ÷� ��û �־����
		VPMath::Matrix offset[1024];
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


	//�����ְ� ���� 100�� ����
	LightData array[100];
	float DirIndex;
	float PointIndex;
	float SpotIndex;
	float pad;
};

struct MaterialData
{
	MaterialData() : albedo(1,1,1,1), metalness(0),roughness(0),ao(0),useNEO(0,0,0,0), useAMRO(0,0,0,0)
	{

	}

	DirectX::XMFLOAT4 useAMRO;	//�ؽ�ó�� ������ �ִ°� albedo, metalic, roughness, AO
	DirectX::XMFLOAT4 useNEO;	//�ؽ�ó�� ������ �ִ°� normal, Emissive, opacity
	DirectX::XMFLOAT4 albedo;
	float metalness;
	float roughness;
	float ao; // Ambient Occlusion
	float pad;
};