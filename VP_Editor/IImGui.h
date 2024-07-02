#pragma once
#include "../include/imgui.h"
#include "../include/ImGuizmo.h"

class IImGui
{
public: 
	IImGui() = default;
	virtual ~IImGui() = default;
	virtual void ImGuiRender()=0;
};

