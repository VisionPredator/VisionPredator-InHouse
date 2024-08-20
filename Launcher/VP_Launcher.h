#pragma once
#include "../VisionpredatorProcess/VPProcess.h"
#include "EventSubscriber.h"

class VP_Launcher : public VPProcess
{
public:
	VP_Launcher(HINSTANCE hInstance, std::string title, int width, int height);
	~VP_Launcher()=default;

	void SetStartScene(std::string FilePath);

};

