#pragma once
#include <Component.h>
#include "InputManager.h"
struct DestroySelfComponent :
	public Component
{
	VP_JSONBODY(DestroySelfComponent, Button)
		KEYBOARDKEY Button = KEYBOARDKEY::F6;
};

