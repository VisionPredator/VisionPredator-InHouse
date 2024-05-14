#pragma once
#include <vector>
#include <string>

#include "SimpleMath.h"

class Node;

struct Key
{
	Key() :time(), value(), rotation(), RotationMatrix(), TranslateMatrix(), ScaleMatrix() {}

	double time;
	DirectX::SimpleMath::Vector3 value;
	DirectX::SimpleMath::Quaternion rotation;
	DirectX::SimpleMath::Matrix RotationMatrix;
	DirectX::SimpleMath::Matrix TranslateMatrix;
	DirectX::SimpleMath::Matrix ScaleMatrix;

};


struct Channel
{
	Channel() {}

	std::wstring nodename;
	Node* node;
	std::vector<Key*> positionkey;
	std::vector<Key*> rotationkey;
	std::vector<Key*> scalingkey;
};


class Animation
{
public:
	Animation();
	~Animation();

	double m_Duration;
	double m_TickFrame;

	std::vector<Channel*> m_Channels;
};

