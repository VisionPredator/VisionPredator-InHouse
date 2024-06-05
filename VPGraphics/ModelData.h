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

	std::shared_ptr<Node> m_RootNode;	//node 전체를 담고 있는 컨테이너

	std::vector<std::shared_ptr<Mesh>> m_Meshes; // VB + IB + BONE + PRIMITIVE
	std::vector<std::shared_ptr<Material>> m_Materials; //SRV
	std::vector<std::shared_ptr<Animation>> m_Animations; //애니메이션 전체를 담고있는 컨테이너



	///여기 데이터를 따로 때서 Modeldata를 갖는 형식으로 해야할까?
	//위치 data
	DirectX::SimpleMath::Matrix world; //게임 세상의 위치
	DirectX::SimpleMath::Matrix local; //캐릭터 자체 로컬


	std::weak_ptr<RenderState>  RS;
	PassState m_pass;

};

