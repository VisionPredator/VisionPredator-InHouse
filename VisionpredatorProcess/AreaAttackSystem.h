#pragma once
#include "System.h"
#include "VisPredComponents.h"

class AreaAttackSystem:public System, public ITriggerable
{
public :

	AreaAttackSystem(std::shared_ptr<SceneManager> sceneManager);
	~AreaAttackSystem()=default;

	// ITriggerable을(를) 통해 상속됨
	void EnterTrigger(std::pair<uint32_t, uint32_t> entitypair) override;
	void ExitTrigger(std::pair<uint32_t, uint32_t> entitypair) override;
	void TriggerAttack(AreaAttackComponent& area ,EnemyComponent& enemy);

};

