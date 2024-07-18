#pragma once
#include <string>
#include<vector>
#include "SimpleMath.h"
#include <map>
#include "MeshFilter.h"

class Node;
struct Key;
class ModelData;
class ResourceManager;

class Animator
{
public:
	Animator(std::weak_ptr<ResourceManager> manager);
	~Animator();
	std::vector<std::shared_ptr<ModelData>> m_AnimationModel;

	void Update(double dt, std::map<uint32_t, std::shared_ptr<RenderData>>& renderlist);

private:

	void UpdateWorld(double dt, std::weak_ptr<ModelData> ob);

	//test
	void UpdateWorld(std::weak_ptr<RenderData> ob);
	void CalcWorld(std::shared_ptr<Node> RootNode);

	DirectX::SimpleMath::Matrix CalcMatrix(double time, std::vector<std::shared_ptr<Key>> channel);
	DirectX::SimpleMath::Matrix CalcRotation(double time, std::vector<std::shared_ptr<Key>> rotationKey);

	void UpdateMatrixPallete(std::shared_ptr<RenderData>& curData);

	std::weak_ptr<ResourceManager> m_ResourceManager;


	//calc nodes
	std::vector<std::shared_ptr<Node>> m_Nodes;


};

