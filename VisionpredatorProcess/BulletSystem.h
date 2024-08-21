#pragma once
#include "../VPEngine/System.h"

class BulletSystem :
    public System,public IFixedUpdatable, public IContactable
{
public:
	BulletSystem(std::shared_ptr<SceneManager> sceneManager);
	~BulletSystem() override = default;


	// IFixedUpdatable을(를) 통해 상속됨
	void FixedUpdate(float deltaTime) override;


	//void EnterCollision(std::pair<uint32_t, uint32_t> entitypair) override;

	//void ExitCollision(std::pair<uint32_t, uint32_t> entitypair) override;


	// IContactable을(를) 통해 상속됨
	void EnterCollision(std::pair<uint32_t, uint32_t> entitypair) override;

	void ExitCollision(std::pair<uint32_t, uint32_t> entitypair) override;

};

