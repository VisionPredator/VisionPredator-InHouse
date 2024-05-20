#pragma once
#include "Resource.h"
#include "MeshFilter.h"

#include <string>

#include "../directxtk/SimpleMath.h"


class Mesh;
class Material;
class Animation;
class Node;
class RenderState;

class ModelData :
    public Resource
{
public:
	ModelData() : m_name(L"need name"), m_RootNode(),RS(nullptr)
	{
	}


	ModelData(ModelData* other)
	{
		this->m_RootNode = other->m_RootNode;
		this->m_Meshes = other->m_Meshes;
		this->m_Materials = other->m_Materials;
		this->m_Animations = other->m_Animations;
	}


	virtual void Release() override {}

	std::wstring m_name;

	Node* m_RootNode;	//node ��ü�� ��� �ִ� �����̳�

	std::vector<Mesh*> m_Meshes; // VB + IB + BONE + PRIMITIVE
	std::vector<Material*> m_Materials; //SRV
	std::vector<Animation*> m_Animations; //�ִϸ��̼� ��ü�� ����ִ� �����̳�

	//��ġ data
	DirectX::SimpleMath::Matrix world; //���� ������ ��ġ
	DirectX::SimpleMath::Matrix local; //ĳ���� ��ü ����


	RenderState* RS;
	PassState m_pass;

};

