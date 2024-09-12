#pragma once
#include <string>
#include<vector>
#include <map>
#include "MeshFilter.h"

class Node;
struct Key;
class ModelData;
class ResourceManager;

class Animator
{
public:
	Animator() = default;
	~Animator() = default;

	std::vector<std::shared_ptr<ModelData>> m_AnimationModel;

	void Initialize(std::weak_ptr<ResourceManager> manager);

	void Update(double dt, std::map<uint32_t, std::shared_ptr<RenderData>>& renderlist);
	const VPMath::Matrix Attachment(std::wstring region);

private:


	void UpdateWorld(std::weak_ptr<RenderData> ob);
	void CalcWorld(std::shared_ptr<Node> RootNode);

	void UpdateMatrixPallete(std::shared_ptr<RenderData>& curData);

	std::weak_ptr<ResourceManager> m_ResourceManager;


	//calc nodes
	std::vector<std::shared_ptr<Node>> m_Nodes;


	VPMath::Matrix socket;

};

