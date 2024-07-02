#include "pch.h"
#include "Node.h"

Node::Node()
{

}

Node::Node(Node& other)
{
	name = other.name;
	m_Local = other.m_Local;
	m_LocalInverse = other.m_LocalInverse;
	m_World=other.m_World;
	m_WorldInverse= other.m_WorldInverse;
	index = other.index;
	
	for (int i = 0; i < other.m_Meshes.size(); i++)
	{
		std::shared_ptr<Mesh> newNode = std::make_shared<Mesh>();
		newNode = other.m_Meshes[i];
		m_Meshes.push_back(newNode);
	}

	std::weak_ptr<Node> m_Parents;
	bool HasParents = false;

	std::vector<std::shared_ptr<Node>> m_Childs;
	std::vector<std::weak_ptr<Bone>> m_Bones;

}

Node& Node::operator=(Node& other)
{
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

}

Node::~Node()
{

}
