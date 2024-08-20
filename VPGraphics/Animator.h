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

private:

	void UpdateWorld(double dt, std::weak_ptr<ModelData> ob);

	//test
	void UpdateWorld(std::weak_ptr<RenderData> ob);
	void CalcWorld(std::shared_ptr<Node> RootNode);

	VPMath::Matrix CalcMatrix(double time, std::vector<std::shared_ptr<Key>> channel);
	VPMath::Matrix CalcRotation(double time, std::vector<std::shared_ptr<Key>> rotationKey);

	void UpdateMatrixPallete(std::shared_ptr<RenderData>& curData);

	std::weak_ptr<ResourceManager> m_ResourceManager;


	//calc nodes
	std::vector<std::shared_ptr<Node>> m_Nodes;


};

