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
	void ClearDecals();

	std::map<std::string, std::vector<decal::Info>>& GetDecals();

private:

	std::map<std::string, std::vector<decal::Info>> m_decals;
};

