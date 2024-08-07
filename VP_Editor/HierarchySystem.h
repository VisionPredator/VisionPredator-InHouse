#pragma once
#include "System.h"
class HierarchySystem :public System
{
public:
	HierarchySystem(std::shared_ptr<SceneManager> sceneManager);
	void ShowEntitys();
	~HierarchySystem() override = default;
	// Helper function to convert a string to lowercase
	std::string ToLower(const std::string& str)
	{
		std::string lowerStr = str;
		std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
		return lowerStr;
	}

	void ShowParentEntity(uint32_t entityID);
	static uint32_t m_SelectedEntityID;
	uint32_t m_RClickedEntityID = 0;
	inline static bool IsItemDoubleClicked = false;
	std::string m_SearchingName{};
	bool m_IsEntityRClicked = false;
};

