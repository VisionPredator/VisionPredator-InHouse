#pragma once
#include <System.h>
class LDHButtonSystem :
	public System, public IUpdatable
{
public:
	LDHButtonSystem(std::shared_ptr<SceneManager> scenemanager);
	// IUpdatable��(��) ���� ��ӵ�
	void Update(float deltaTime) override;

};

