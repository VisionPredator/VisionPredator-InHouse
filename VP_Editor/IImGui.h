#pragma once

class IImGui
{
public: 
	IImGui() = default;
	virtual ~IImGui() = default;
	virtual void ImGuiRender()=0;
};

