#pragma once
#include "vpGraphics.h"
#include <queue>

///2024.10.18
///���¿�
///Decalobject�� ������ �Ŵ��� Ŭ����

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

