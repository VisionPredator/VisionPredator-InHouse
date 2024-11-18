#pragma once
#include <System.h>
class LDHButtonSystem :
	public System, public IUpdatable
{
public:
	LDHButtonSystem(std::shared_ptr<SceneManager> scenemanager);
	// IUpdatable을(를) 통해 상속됨
	void Update(float deltaTime) override;

};

