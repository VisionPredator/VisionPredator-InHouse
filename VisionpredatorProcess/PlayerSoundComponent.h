#pragma once
#include <Component.h>
 struct PlayerSoundComponent:
	 public Component
 {
	 PlayerSoundComponent();
	 VP_JSONBODY(PlayerSoundComponent, Volume_Walk, Volume_Run, Volume_Jump, Volume_Landing, Volume_Slide, Volume_Sit, Volume_Hurt, Volume_Death, Volume_Dash, Volume_Transformation, Volume_GunDraw, Volume_GunThrow
		 , Volume_Sword1, Volume_Sword2, Volume_Sword3, Volume_VPAttack1, Volume_VPAttack2, SoundKey_Walk1, SoundKey_Walk2, SoundKey_Run1, SoundKey_Run2, SoundKey_Jump, SoundKey_Landing, SoundKey_Slide, SoundKey_Sit, SoundKey_Hurt, SoundKey_Death, SoundKey_GunDraw, SoundKey_GunThrow,SoundKey_Sword1, SoundKey_Sword2, SoundKey_Sword3, SoundKey_Dash, SoundKey_Transformation);
	 int Volume_Walk{ 25 };
	 int Volume_Run{ 25 };
	 int Volume_Jump{ 40 };
	 int Volume_Landing{ 40 };
	 int Volume_Slide{ 100 };
	 int Volume_Sit{ 100 };
	 int Volume_Hurt{ 30 };
	 int Volume_Death{ 30 };
	 int Volume_Dash{ 30 };
	 int Volume_Transformation{ 30 };
	 int Volume_GunDraw{ 30 };
	 int Volume_GunThrow{ 30 };
	 int Volume_Sword1{ 30 };
	 int Volume_Sword2{ 30 };
	 int Volume_Sword3{ 30 };
	 int Volume_VPAttack1{ 30 };
	 int Volume_VPAttack2{ 30 };
	 std::string SoundKey_Walk1 = "Player_Walk1";
	 std::string SoundKey_Walk2 = "Player_Walk2";
	 std::string SoundKey_Run1 = "Player_Run1";
	 std::string SoundKey_Run2 = "Player_Run2";
	 std::string SoundKey_Jump = "Player_Jump";
	 std::string SoundKey_Landing = "Player_Landing";
	 std::string SoundKey_Slide = "Player_Slide";
	 std::string SoundKey_Sit = "Player_Sit";
	 std::string SoundKey_Hurt = "Player_Hurt";
	 std::string SoundKey_Death = "Player_Death";
	 std::string SoundKey_GunDraw = "Weapon_Draw";
	 std::string SoundKey_GunThrow = "Weapon_Throwing";
	 std::string SoundKey_Sword1 = "Sword_Attack1";
	 std::string SoundKey_Sword2 = "Sword_Attack3";
	 std::string SoundKey_Sword3 = "Sword_Attack2";
	 std::string SoundKey_VPAttack1 = "VP_Attack1";
	 std::string SoundKey_VPAttack2 = "VP_Attack2";
	 std::string SoundKey_Dash = "VP_Dash";
	 std::string SoundKey_Transformation = "Change";


     bool Played_Walk1{};
     bool Played_Jump{};
     bool Played_Slide{};
     bool Played_Sit{};
     bool Played_Run1{};


};

