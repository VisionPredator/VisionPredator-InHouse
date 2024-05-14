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

	std::vector<Mesh*> m_Meshes;

	Node* m_Parents;
	bool HasParents = false;

	std::vector<Node*> m_Childs;
	std::vector<Bone*> m_Bones;

	virtual void Release() override;
};

