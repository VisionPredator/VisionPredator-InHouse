#pragma once
namespace VisPred
{
	namespace Engine
	{
		struct AniBlendData
		{
			uint32_t EntityID;
			int Index{};
			float Speed{};
			float TransitionTime{};
			bool Loop;
		};
	}
}
