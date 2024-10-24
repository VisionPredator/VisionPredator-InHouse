#pragma once
#include "../VPEngine/System.h"

class BulletSystem :
    public System,public IFixedUpdatable, public IContactable,public IStartable,public IPhysicable
{
public:
	BulletSystem(std::shared_ptr<SceneManager> sceneManager);
	~BulletSystem() override = default;


	// IFixedUpdatable을(를) 통해 상속됨
	void FixedUpdate(float deltaTime) override {};


	//void EnterCollision(std::pair<uint32_t, uint32_t> entitypair) override;

	//void ExitCollision(std::pair<uint32_t, uint32_t> entitypair) override;


	// IContactable을(를) 통해 상속됨
	void EnterCollision(std::pair<uint32_t, uint32_t> entitypair) override;
	void ApplyDamage(Entity& bullet, Entity& Other);
	void ExitCollision(std::pair<uint32_t, uint32_t> entitypair) override {};


	// IStartable을(를) 통해 상속됨
	void Initialize() override;

	void Start(uint32_t gameObjectId) override;

	void Finish(uint32_t gameObjectId) override {};

	void Finalize() override {};


	// IPhysicable을(를) 통해 상속됨
	void PhysicsUpdate(float deltaTime) override {};

	void PhysicsLateUpdate(float deltaTime) override {};

};

