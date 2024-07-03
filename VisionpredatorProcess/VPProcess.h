#pragma once
#include "../VPEngine/VPEngine.h"
class VPProcess :public VPEngine
{
	VPProcess(HINSTANCE hInstance, std::string title, int width, int height);
	~VPProcess();

};

