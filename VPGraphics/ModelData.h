#pragma once
#include "Resource.h"
#include "MeshFilter.h"

#include <string>

#include "../directxtk/SimpleMath.h"
#include<memory>

class Mesh;
class Material;
class Animation;
class Node;
class RenderState;

class ModelData :
	public Resource, public std::enable_shared_from_this<ModelData>
{
public:
	ModelData() : m_name(L"need name"), m_RootNode(),RS()
	{
	}

	~ModelData();

	ModelData(std::shared_ptr<ModelData> other)
	{
		this->m_RootNode = other->m_RootNode;
		this->m_Meshes = other->m_Meshes;
		this->m_Materials = other->m_Materials;
		this->m_Animations = other->m_Animations;
	}


	virtual void Release() override;

	std::wstring m_name;

	std::shared_ptr<Node> m_RootNode;	//node ��ü�� ��� �ִ� �����̳�

	std::vector<std::shared_ptr<Mesh>> m_Meshes; // VB + IB + BONE + PRIMITIVE
	std::vector<std::shared_ptr<Material>> m_Materials; //SRV
	std::vector<std::shared_ptr<Animation>> m_Animations; //�ִϸ��̼� ��ü�� ����ִ� �����̳�



	///���� �����͸� ���� ���� Modeldata�� ���� �������� �ؾ��ұ�?
	//��ġ data
	DirectX::SimpleMath::Matrix world; //���� ������ ��ġ
	DirectX::SimpleMath::Matrix local; //ĳ���� ��ü ����


	std::weak_ptr<RenderState>  RS;
	PassState m_pass;

};

