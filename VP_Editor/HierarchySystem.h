#pragma once
#include "System.h"
class HierarchySystem :public System
{
public:
	HierarchySystem(std::shared_ptr<SceneManager> sceneManager);
	void ShowEntitys();
	~HierarchySystem() override = default;

	void ShowParentEntity(uint32_t entityID);
	static uint32_t m_SelectedEntityID;
	uint32_t m_RClickedEntityID = 0;
	inline static bool IsItemDoubleClicked = false;

	bool m_IsEntityRClicked = false;
};

