#pragma once
#include "VPEngine.h"

class GameProcessor :public VPEngine
{
public:
	GameProcessor(HINSTANCE hInstance, std::string title, int width, int height);
	~GameProcessor();

};
