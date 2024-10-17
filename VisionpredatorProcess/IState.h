#pragma once
#include "Component.h"
#include "../VPGraphics/Log.h"
class IState
{
public:
	virtual ~IState() = default;

	virtual void Enter(uint32_t entityID) { Log::GetClientLogger()->info("Enter IState"); }
	virtual void Update(uint32_t entityID, float deltaTime) { Log::GetClientLogger()->info("Update IState"); }
	virtual void Exit(uint32_t entityID) { Log::GetClientLogger()->info("Exit IState"); }
};

