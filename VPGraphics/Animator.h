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

	void Update(double dt, std::map<std::wstring, std::pair<PassState, ModelData*>>& models);

private:
	std::vector<Object*> m_PlayList;

	void UpdateWorld(double dt, ModelData* ob);
	void CalcWorld(Node* RootNode);


	DirectX::SimpleMath::Matrix CalcMatrix(double time, std::vector<Key*> channel);
	DirectX::SimpleMath::Matrix CalcRotation(double time, std::vector<Key*> rotationKey);

	void UpdateMatrixPallete(ModelData* ob);


};

