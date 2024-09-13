#pragma once
#include <vector>
#include <string>


class Node;

struct Key
{
	Key() :time(), value(), rotation(), RotationMatrix(), TranslateMatrix(), ScaleMatrix() {}

	double time;
	VPMath::Vector3 value;
	VPMath::Quaternion rotation;
	VPMath::Matrix RotationMatrix;
	VPMath::Matrix TranslateMatrix;
	VPMath::Matrix ScaleMatrix;
	VPMath::Matrix total;
};


struct Channel
{
	Channel() {}

	std::wstring nodename;
	std::weak_ptr<Node> node;
	std::vector<std::shared_ptr<Key>> positionkey;
	std::vector<std::shared_ptr<Key>> rotationkey;
	std::vector<std::shared_ptr<Key>> scalingkey;

	std::vector<std::pair<float ,VPMath::Matrix>> totals;
};


class Animation
{
public:
	Animation();
	~Animation();

	double m_Duration;
	double m_TickFrame;

	std::vector<std::shared_ptr<Channel>> m_Channels;
	//std::vector<std::map<std::wstring, std::vector<std::shared_ptr<Key>>>> Animationkey;

};

