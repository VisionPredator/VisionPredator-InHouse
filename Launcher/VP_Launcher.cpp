#include "pch.h"
#include "VP_Launcher.h"
#include "InputManager.h"
#include "../VPEngine/SceneManager.h"
VP_Launcher::VP_Launcher(HINSTANCE hInstance, std::string title, int width, int height, bool ismenubar) :VPProcess(hInstance, title, width, height, ismenubar)
{
	InputManager::GetInstance().SetClipMode(TRUE);
	ShowCursor(FALSE);
}


void VP_Launcher::SetStartScene(std::string FilePath)
{
	m_SceneManager->ChangeScene(FilePath);
}

