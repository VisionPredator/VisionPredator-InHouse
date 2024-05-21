#pragma once
#include "Resource.h"
#include "SimpleMath.h"


class Mesh;
class Bone;

class Node :
    public Resource
{
public:
	Node();
	~Node();

	std::wstring name;

	DirectX::SimpleMath::Matrix m_Local;
	DirectX::SimpleMath::Matrix m_LocalInverse;
	DirectX::SimpleMath::Matrix m_World;
	DirectX::SimpleMath::Matrix m_WorldInverse;

	unsigned int index;

	std::vector<std::weak_ptr<Mesh>> m_Meshes;

	std::weak_ptr<Node> m_Parents;
	bool HasParents = false;

	std::vector<std::shared_ptr<Node>> m_Childs;
	std::vector<std::weak_ptr<Bone>> m_Bones;

	virtual void Release() override;
};

