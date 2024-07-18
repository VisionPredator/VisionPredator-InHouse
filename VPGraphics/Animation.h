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
	DirectX::SimpleMath::Matrix total;
};


struct Channel
{
	Channel() {}

	std::wstring nodename;
	std::weak_ptr<Node> node;
	std::vector<std::shared_ptr<Key>> positionkey;
	std::vector<std::shared_ptr<Key>> rotationkey;
	std::vector<std::shared_ptr<Key>> scalingkey;
	std::vector<DirectX::SimpleMath::Matrix> totals;
};


class Animation
{
public:
	Animation();
	~Animation();

	double m_Duration;
	double m_TickFrame;

	std::vector<std::shared_ptr<Channel>> m_Channels;
	std::vector<std::map<std::wstring, std::vector<std::shared_ptr<Key>>>> Animationkey;

};

