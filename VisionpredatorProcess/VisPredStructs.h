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
		enum class PlayerAni
		{
			 ToAttack_Pistol	=0
			,ToAttack_Rifle		=1
			,ToAttack_ShotGun	=2
			,ToIdle01_Pistol	=3
			,ToIdle01_Rifle		=4
			,ToIdle01_ShotGun	=5
			,ToIdle02_Pistol	=6
			,ToIdle02_Rifle		=7
			,ToIdle02_ShotGun	=8
			,Tohook_Sword		=9
			,Tohook_Pistol		=10
			,Tohook_Rifle		=11
			,Tohook_ShotGun		=12
			,Tointeraction		=13
			,ToAttack1_Sword	=14
			,ToAttack2_Sword	=15
			,ToAttack3_Sword	=16
			,ToIdle01_Sword		=17
			,ToIdle02_Sword		=18
			,ToThrow_Pistol		=19
			,ToThrow_Rifle		=20
			,ToThrow_ShotGun	=21
			,ToVP_attack_L		=22
			,ToVP_attack_R		=23
			,ToVP_Idle			=24
			,ToVP_dash			=25
			,ToVP_jump			=26
			,ToVP_run			=27
			,ToVP_draw			=28
		};

		// TODO: Enemy FSM �� Animation ���� Enum ���� �����ؾ��Ѵ�.
		enum class EnemyState // �ӽ� �̸�. EnemyState �� ��ü
		{
			Idle,
			Chase,
			Patrol,			// �÷��̾� �߰��ϴ� ������ ��� �����ð� Patrol ���¸� ����
			Dead,

			End
		};
		// TODO: ���� �ൿ EnemyState �� ������ �ڴ�.
		enum class SecondState	// �ӽ� �̸�.
		{
			ReturnToSpawn,
			Hit,
			// etc...
		};

		//fbx�� ����� ������ �°� enum�� �������
		enum class EnemyAni
		{
			ATTACK = 0
			, IDLE = 1
			, CHASE = 2
			, JUMP = 3
			, WALK = 4
			, DIE = 5
			, ATTACKED = 6
			, BACKWALK = 7
			, DESTROY = 8
			, NONE = 9
			, END = 10
		};
	}

}
