#pragma once
#include <Component.h>
 struct PlayerSoundComponent:
    public Component
{
     PlayerSoundComponent();
	 VP_JSONBODY(PlayerSoundComponent, Volume_Walk, Volume_Run, Volume_Jump, Volume_Slide, Volume_Sit, Volume_Hurt, WalkSoundKey1, WalkSoundKey2, RunSoundKey1, RunSoundKey2, JumpSoundkey, SlideSoundkey, SitSoundKey, HurtSoundKey);
     float Volume_Walk{ 0.05000000074505806f };
     float Volume_Run{ 0.02500000037252903f };
     float Volume_Jump{ 0.03999999910593033f };
     float Volume_Slide{ 0.20000000298023224f };
     float Volume_Sit{ 0.4000000059604645f };
     float Volume_Hurt{ 0.1f };
     std::string WalkSoundKey1 = "Player_Walk1";
     std::string WalkSoundKey2 = "Player_Walk2";
     std::string RunSoundKey1 = "Player_Run1";
     std::string RunSoundKey2 = "Player_Run2";
     std::string JumpSoundkey = "Player_Jump";
     std::string SlideSoundkey = "Player_Slide";
     std::string SitSoundKey = "Player_Sit";
     std::string HurtSoundKey = "Player_Hurt";
     bool Played_Walk1{};
     bool Played_Jump{};
     bool Played_Slide{};
     bool Played_Sit{};
     bool Played_Run1{};


};

