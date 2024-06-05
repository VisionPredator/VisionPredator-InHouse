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
	ModelData();
	~ModelData();

	ModelData(std::shared_ptr<ModelData> other);


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

