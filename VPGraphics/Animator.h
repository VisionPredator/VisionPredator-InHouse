#pragma once
#include <string>
#include<vector>
#include "SimpleMath.h"
#include <map>
#include "MeshFilter.h"

class Node;
struct Key;
class ModelData;

class Animator
{
public:
	Animator();
	~Animator();
	std::vector<std::shared_ptr<ModelData>> m_AnimationModel;

	void Update(double dt, std::vector<std::shared_ptr<ModelData>>& models);

private:

	void UpdateWorld(double dt, std::shared_ptr<ModelData> ob);
	void CalcWorld(std::shared_ptr<Node> RootNode);


	DirectX::SimpleMath::Matrix CalcMatrix(double time, std::vector<std::shared_ptr<Key>> channel);
	DirectX::SimpleMath::Matrix CalcRotation(double time, std::vector<std::shared_ptr<Key>> rotationKey);

	void UpdateMatrixPallete(std::shared_ptr<ModelData> ob);


};

