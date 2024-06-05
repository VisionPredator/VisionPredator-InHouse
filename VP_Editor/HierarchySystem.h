#pragma once
#include "System.h"
class HierarchySystem :public System
{
public:
	HierarchySystem(SceneManager* sceneManager);
	void ShowEntitys();
	~HierarchySystem() override = default;

	void ShowParentEntity(uint32_t entityID);
	uint32_t m_SelectedNumber = 0;
	bool m_IsHovered = false;
};

