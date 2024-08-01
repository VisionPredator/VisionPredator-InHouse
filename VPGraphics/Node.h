#pragma once
#include <memory>

class Mesh;
class Bone;

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

	std::vector<std::weak_ptr<Mesh>> m_Meshes;

	std::weak_ptr<Node> m_Parents;
	bool HasParents = false;

	std::vector<std::shared_ptr<Node>> m_Childs;
	std::vector<std::weak_ptr<Bone>> m_Bones;

};

