#pragma once
#include <memory>

class Mesh;
class Bone;

//사실상 얘가 본이네... 
class Node
{
public:
	Node();
	~Node();

	std::wstring name;

	VPMath::Matrix m_Local;
	VPMath::Matrix m_LocalInverse;
	VPMath::Matrix m_World;
	VPMath::Matrix m_WorldInverse;

	unsigned int index;
	unsigned int parentsindex;

	std::vector<std::weak_ptr<Mesh>> m_Meshes;

	std::weak_ptr<Node> m_Parents;
	bool HasParents = false;

	std::vector<std::shared_ptr<Node>> m_Childs;
};

