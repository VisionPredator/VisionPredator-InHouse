#pragma once
#include <Component.h>
struct IdentityRemoverComponent :
	public Component
{
	VP_JSONBODY(IdentityRemoverComponent, RemoveIdentitys);
	std::vector < std::string > RemoveIdentitys{};

};

