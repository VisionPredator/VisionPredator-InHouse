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
		enum class QuestType
		{
			VPMOVE,
			VPJUMP,
			VPDASH,
			VPCHANGE,
			PLAYERRUN,
			PLAYERJUMP,
			PLAYERCROUCH,
			PLAYERSLIDE,
			PLAYERATTACK,
			PLAYERINTERECT,
			PLAYERPICKUP,
			PLAYERSHOOT,
			PLAYERTHROW,

		};
		enum class GunRecoilMode
		{
			ReturnToMiddle,
			ReturnToEndAim

		};
		enum class PlayerMelee
		{
			Sword_First,
			Sword_Second,
			Sword_Third,
			Sword_Fourth,
			VP_Left,
			VP_Right,
			END
		};
		enum class PlayerFSM
		{
			 IDLE
			,WALK
			,RUN
			,CROUCH
			,Dash_Slide
			,JUMP
			,Transformation
			,DIE
			,DIE_END
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

		};
		enum class VPAni
		{
			ToVP_attack_L 
			, ToVP_attack_R 
			, ToVP_Idle 
			, ToVP_dash
			, ToVP_jump
			, ToVP_run
			, ToVP_draw 
		};
		enum class EnemyStates // �ӽ� �̸�. EnemyStates �� ��ü
		{
			Idle,
			Chase,
			Patrol,			// �÷��̾� �߰��ϴ� ������ ��� �����ð� Patrol ���¸� ����
			Dead,

			End
		};
		// TODO: ���� �ൿ EnemyStates �� �����߰ڴ�. �����̳ʿ� ������?
		enum class SecondState	// �ӽ� �̸�.
		{
			ReturnToSpawn,
			Hit,
			// etc...
		};

		// enum ���� == fbx�� ����� �ִϸ��̼� ����
		// NPC �� Ÿ�� ��� �ִϸ��̼� ������ �� �ִϸ��̼� ���� ����
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
			, ATTACK_IDLE = 8
			, END = 9
		};
	}

}
