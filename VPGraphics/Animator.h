#pragma once
#include <string>
#include<vector>
#include "SimpleMath.h"
#include <map>
#include "MeshFilter.h"

class Object;
class Node;
struct Key;
class ModelData;

class Animator
{
public:
	Animator();
	~Animator();

	void Update(double dt, std::map<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>& models);

private:
	std::vector<Object*> m_PlayList;

	void UpdateWorld(double dt, std::shared_ptr<ModelData> ob);
	void CalcWorld(std::shared_ptr<Node> RootNode);


	DirectX::SimpleMath::Matrix CalcMatrix(double time, std::vector<std::shared_ptr<Key>> channel);
	DirectX::SimpleMath::Matrix CalcRotation(double time, std::vector<std::shared_ptr<Key>> rotationKey);

	void UpdateMatrixPallete(std::shared_ptr<ModelData> ob);


};

