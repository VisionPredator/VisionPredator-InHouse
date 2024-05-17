#pragma once
#include "Resource.h"

#include <string>

#include "../directxtk/SimpleMath.h"


class Mesh;
class Material;
class Animation;
class Node;

class ModelData :
    public Resource
{
public:
	ModelData() : m_name(L"need name"), m_RootNode()
	{
	}

	virtual void Release() override {}

	std::wstring m_name;

	Node* m_RootNode;	//node 전체를 담고 있는 컨테이너

	std::vector<Mesh*> m_Meshes; // VB + IB + BONE + PRIMITIVE
	std::vector<Material*> m_Materials; //SRV
	std::vector<Animation*> m_Animations; //애니메이션 전체를 담고있는 컨테이너

	//위치 data
	DirectX::SimpleMath::Matrix world; //게임 세상의 위치
	DirectX::SimpleMath::Matrix local; //캐릭터 자체 로컬
};

