#include "pch.h"
#include "VP_Launcher.h"
#include "../VPEngine/SceneManager.h"
VP_Launcher::VP_Launcher(HINSTANCE hInstance, std::string title, int width, int height) :VPProcess(hInstance, title, width, height)
{
}


void VP_Launcher::SetStartScene(std::string FilePath)
{
	m_SceneManager->ChangeScene(FilePath);
}

