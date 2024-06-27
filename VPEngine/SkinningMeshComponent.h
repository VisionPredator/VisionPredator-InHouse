#pragma once
#include "Component.h"

enum class TestEnum
{
	a,
	b,
	c,
	d,
};
NLOHMANN_JSON_SERIALIZE_ENUM(TestEnum, {
{TestEnum::a, "a"},
{TestEnum::b, "b"},
{TestEnum::c, "c"},
{TestEnum::d, "d"}
	});
struct SkinningMeshComponent :
		public Component
	{
		SkinningMeshComponent();

		VP_JSONBODY(SkinningMeshComponent, Name, Test)

		std::string Name;
		TestEnum Test{};
	};

	