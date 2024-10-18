#pragma once
#include "Component.h"
#include "../PhysxEngine/IPhysx.h"
#include "../VPGraphics/Log.h"

class IState
{
public:
	virtual void Enter(const std::shared_ptr<Component>& component) { Log::GetClientLogger()->info("Enter IState"); }
	virtual void Update(const std::shared_ptr<Component>& component, float deltaTime) { Log::GetClientLogger()->info("Update IState"); }
	virtual void Exit(const std::shared_ptr<Component>& component) { Log::GetClientLogger()->info("Exit IState"); }
};

