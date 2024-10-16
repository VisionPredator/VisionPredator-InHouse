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

		enum class EnemyState // 임시 이름. EnemyState 로 교체
		{
			Idle,
			Chase,
			Patrol,			// 플레이어 추격하다 놓쳣을 경우 일정시간 Patrol 상태를 유지
			Dead,

			End
		};
		// TODO: 하위 행동 EnemyState 를 만들어야겠다. 컨테이너에 넣을까?
		enum class SecondState	// 임시 이름.
		{
			ReturnToSpawn,
			Hit,
			// etc...
		};

		// enum 순서 == fbx에 저장된 애니메이션 순서
		// NPC 세 타입 모두 애니메이션 순서와 총 애니메이션 수는 동일
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
			, END = 8
		};
	}

}
