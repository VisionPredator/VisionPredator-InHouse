#pragma once
#include "System.h"
#include "AreaAttackComponent.h"
#include "EnemyComponent.h"
class AreaAttackSystem:public System, public ITriggerable,public IFixedUpdatable,public  IStartable
{
public :

	AreaAttackSystem(std::shared_ptr<SceneManager> sceneManager);
	~AreaAttackSystem()=default;

	// ITriggerable��(��) ���� ��ӵ�
	void EnterTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second) override;
	void ExitTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second) override;
	void TriggerAttack(AreaAttackComponent& area ,EnemyComponent& enemy);


	// IFixedUpdatable��(��) ���� ��ӵ�
	void FixedUpdate(float deltaTime) override;

	// IStartable��(��) ���� ��ӵ�
	void Initialize() override;

	void Start(uint32_t gameObjectId) override;

	void Finish(uint32_t gameObjectId) override;

	void Finalize() override;

};

