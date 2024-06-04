#include "pch.h"
#include "Toolbar.h"

Toolbar::Toolbar()
{
}

void Toolbar::ImGuiRender()
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Light0");
	}
}
