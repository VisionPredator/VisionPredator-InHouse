#pragma once
#include "../VPEngine/VPEngine.h"
class VPProcess :public VPEngine
{
public:
	VPProcess(HINSTANCE hInstance, std::string title, int width, int height,bool ismenubar);
	~VPProcess();
	void Initialize();
	void AddSystems();
	virtual void Update();
	virtual void Render();


};

