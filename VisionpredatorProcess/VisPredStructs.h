#pragma once
#include <bitset>
namespace VisPred
{
	namespace Game
	{
		enum class GunType
		{
			PISTOL,
			SHOTGUN,
			RIFLE,
			NONE,
			END

		};

		enum class EFSM
		{
			IDLE,
			MOVE,
			JUMP,
			CHARGE,
			ATTACK,
			DIE,
			DESTROY,
			NONE,
			END
		};
	}

}




