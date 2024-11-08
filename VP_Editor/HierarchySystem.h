#pragma once
#include "System.h"
#include"EventSubscriber.h"
class HierarchySystem :public System,public EventSubscriber
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
	void ShowEntity(uint32_t entityID);
	static uint32_t m_SelectedEntityID;
	uint32_t m_RClickedEntityID = 0;
	inline static bool IsItemDoubleClicked = false;
	bool ShowDelete{};
	std::string m_SearchingName{};
	uint32_t m_EntityCount{};
	bool m_IsEntityRClicked = false;
	bool IsPlayMode{};

	void OnPlayButton(std::any none);
	void OnStopButton(std::any none);
};

