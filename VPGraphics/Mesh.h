#pragma once
#include "Resource.h"

#include <vector>
#include <string>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Vertex.h"

#include "Node.h"
#include "CBuffer.h"

class Bone
{
public:
	Bone() : name(), offsetMatrix(), offsetMatrixInverse(), NodeMatrix(), Boneindex(), vertexid(), weight(), node(nullptr)
	{}

	std::wstring name;
	DirectX::SimpleMath::Matrix offsetMatrix;
	DirectX::SimpleMath::Matrix offsetMatrixInverse;
	DirectX::SimpleMath::Matrix NodeMatrix;
	int Boneindex;
	int vertexid;
	float weight;
	std::vector<float> weights;
	std::vector<int> vertexids;
	Node* node;
};

class Mesh : public Resource
{
public:
	Mesh();

	virtual void Release() override;

	ID3D11Buffer* VB();
	ID3D11Buffer** GetAddressVB();
	UINT* VBSize();
	UINT* VBOffset();

	ID3D11Buffer* IB();
	ID3D11Buffer** GetAddressIB();
	UINT IBCount();

	VertexBuffer* m_VB;
	IndexBuffer* m_IB;
	D3D11_PRIMITIVE_TOPOLOGY m_primitive;


	virtual bool IsSkinned() abstract;

};

class StaticMesh : public Mesh
{
public:
	StaticMesh();
	~StaticMesh();

	virtual bool IsSkinned() override final;

private:
};

class SkinnedMesh : public Mesh
{
public:
	SkinnedMesh();
	~SkinnedMesh();
	virtual bool IsSkinned() override final;

	Node* m_node;
	std::vector<Bone*> m_BoneData;
	MatrixPallete* Matrix_Pallete;

private:


};