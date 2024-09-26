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
			WALK,
			RUN,
			CROUCH,
			SLIDE,
			JUMP,
			CHARGE,
			ATTACK,
			DIE,
			DESTROY,
			NONE,
			END
		};

		//fbx�� ����� ������ �°� enum�� �������
		enum class EnemyState
		{
			ATTACK,
			IDLE,
			CHASE,
			JUMP,
			WALK,
			DIE,
			ATTACKED,
			BACKWALK,
			DESTROY,
			NONE,
			END
		};
	}

}
