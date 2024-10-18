#pragma once
#include "vpGraphics.h"
#include <queue>

///2024.10.18
///유승운
///Decalobject를 관리할 매니저 클래스

class DecalManager
{
public:
	DecalManager();

	void Initialize();
	void AddTask(decal::Info info);
	std::queue<decal::Info> GetTask() const;


private:
	std::queue<decal::Info> m_decals;
};

