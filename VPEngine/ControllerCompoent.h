#pragma once
#include "Component.h"
#include "../PhysxEngine/VPPhysicsStructs.h"
struct ControllerCompoent :
    public Component
{
    VP_JSONBODY(ControllerCompoent, Contollerinfo, CapsuleControllerinfo)
    
	VPPhysics::ControllerInfo Contollerinfo;
	VPPhysics::CapsuleControllerInfo CapsuleControllerinfo;



};

