#pragma once
#include <string>
#include<vector>
#include "SimpleMath.h"
#include <map>

class Object;
class Node;
struct Key;

class Animator
{
public:
	Animator();
	~Animator();

	void Update(double dt, std::map<std::wstring, Object*>& objects);

private:
	std::vector<Object*> m_PlayList;

	void UpdateWorld(double dt, Object* ob);
	void CalcWorld(Node* RootNode);


	DirectX::SimpleMath::Matrix CalcMatrix(double time, std::vector<Key*> channel);
	DirectX::SimpleMath::Matrix CalcRotation(double time, std::vector<Key*> rotationKey);

	void UpdateMatrixPallete(Object* ob);


};

