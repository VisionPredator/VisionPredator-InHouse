#include "pch.h"
#include "DataRegister_Vispred.h"
#include "VisPredComponents.h"
#include "VisPredStructs.h"

void VispredRegister::Register_Metadata()
{
	VispredRegister::Register_Components();
	VispredRegister::Register_EnumClass();
	VispredRegister::Register_Structs();
	VispredRegister::Register_Value();
	VispredRegister::Register_VPMath();
}
void Register_KeyBoardKey()
{
	entt::meta<KEYBOARDKEY>().prop(Reflection::Prop::Name, "KEYBOARDKEY").conv<std::underlying_type_t<KEYBOARDKEY>>().data<KEYBOARDKEY::ESCAPE >("KEYBOARDKEY::ESCAPE"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::ESCAPE").data<KEYBOARDKEY::NUM1 >("KEYBOARDKEY::NUM1"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUM1").data<KEYBOARDKEY::NUM2 >("KEYBOARDKEY::NUM2"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUM2").data<KEYBOARDKEY::NUM3 >("KEYBOARDKEY::NUM3"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUM3").data<KEYBOARDKEY::NUM4 >("KEYBOARDKEY::NUM4"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUM4").data<KEYBOARDKEY::NUM5 >("KEYBOARDKEY::NUM5"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUM5").data<KEYBOARDKEY::NUM6 >("KEYBOARDKEY::NUM6"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUM6").data<KEYBOARDKEY::NUM7 >("KEYBOARDKEY::NUM7"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUM7").data<KEYBOARDKEY::NUM8 >("KEYBOARDKEY::NUM8"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUM8").data<KEYBOARDKEY::NUM9 >("KEYBOARDKEY::NUM9"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUM9").data<KEYBOARDKEY::NUM0 >("KEYBOARDKEY::NUM0"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUM0").data<KEYBOARDKEY::MINUS >("KEYBOARDKEY::MINUS"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::MINUS").data<KEYBOARDKEY::EQUALS >("KEYBOARDKEY::EQUALS"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::EQUALS").data<KEYBOARDKEY::BACK >("KEYBOARDKEY::BACK"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::BACK").data<KEYBOARDKEY::TAB >("KEYBOARDKEY::TAB"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::TAB").data<KEYBOARDKEY::Q >("KEYBOARDKEY::Q"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::Q").data<KEYBOARDKEY::W >("KEYBOARDKEY::W"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::W").data<KEYBOARDKEY::E >("KEYBOARDKEY::E"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::E").data<KEYBOARDKEY::R >("KEYBOARDKEY::R"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::R").data<KEYBOARDKEY::T >("KEYBOARDKEY::T"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::T").data<KEYBOARDKEY::Y >("KEYBOARDKEY::Y"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::Y").data<KEYBOARDKEY::U >("KEYBOARDKEY::U"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::U").data<KEYBOARDKEY::I >("KEYBOARDKEY::I"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::I").data<KEYBOARDKEY::O >("KEYBOARDKEY::O"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::O").data<KEYBOARDKEY::P >("KEYBOARDKEY::P"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::P").data<KEYBOARDKEY::LBRACKET >("KEYBOARDKEY::LBRACKET"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::LBRACKET").data<KEYBOARDKEY::RBRACKET >("KEYBOARDKEY::RBRACKET"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::RBRACKET").data<KEYBOARDKEY::ENTER >("KEYBOARDKEY::ENTER"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::ENTER").data<KEYBOARDKEY::LCONTROL >("KEYBOARDKEY::LCONTROL"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::LCONTROL").data<KEYBOARDKEY::A >("KEYBOARDKEY::A"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::A").data<KEYBOARDKEY::S >("KEYBOARDKEY::S"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::S").data<KEYBOARDKEY::D >("KEYBOARDKEY::D"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::D").data<KEYBOARDKEY::F >("KEYBOARDKEY::F"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::F").data<KEYBOARDKEY::G >("KEYBOARDKEY::G"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::G").data<KEYBOARDKEY::H >("KEYBOARDKEY::H"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::H").data<KEYBOARDKEY::J >("KEYBOARDKEY::J"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::J").data<KEYBOARDKEY::K >("KEYBOARDKEY::K"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::K").data<KEYBOARDKEY::L >("KEYBOARDKEY::L"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::L").data<KEYBOARDKEY::SEMICOLON >("KEYBOARDKEY::SEMICOLON"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::SEMICOLON").data<KEYBOARDKEY::APOSTROPHE >("KEYBOARDKEY::APOSTROPHE"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::APOSTROPHE").data<KEYBOARDKEY::GRAVE >("KEYBOARDKEY::GRAVE"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::GRAVE").data<KEYBOARDKEY::LSHIFT >("KEYBOARDKEY::LSHIFT"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::LSHIFT").data<KEYBOARDKEY::BACKSLASH >("KEYBOARDKEY::BACKSLASH"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::BACKSLASH").data<KEYBOARDKEY::Z >("KEYBOARDKEY::Z"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::Z").data<KEYBOARDKEY::X >("KEYBOARDKEY::X"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::X").data<KEYBOARDKEY::C >("KEYBOARDKEY::C"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::C").data<KEYBOARDKEY::V >("KEYBOARDKEY::V"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::V").data<KEYBOARDKEY::B >("KEYBOARDKEY::B"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::B").data<KEYBOARDKEY::N >("KEYBOARDKEY::N"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::N").data<KEYBOARDKEY::M >("KEYBOARDKEY::M"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::M").data<KEYBOARDKEY::COMMA >("KEYBOARDKEY::COMMA"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::COMMA").data<KEYBOARDKEY::PERIOD >("KEYBOARDKEY::PERIOD"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::PERIOD").data<KEYBOARDKEY::SLASH >("KEYBOARDKEY::SLASH"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::SLASH").data<KEYBOARDKEY::RSHIFT >("KEYBOARDKEY::RSHIFT"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::RSHIFT").data<KEYBOARDKEY::MULTIPLY >("KEYBOARDKEY::MULTIPLY"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::MULTIPLY").data<KEYBOARDKEY::LMENU >("KEYBOARDKEY::LMENU"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::LMENU").data<KEYBOARDKEY::SPACE >("KEYBOARDKEY::SPACE"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::SPACE").data<KEYBOARDKEY::CAPITAL >("KEYBOARDKEY::CAPITAL"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::CAPITAL").data<KEYBOARDKEY::F1 >("KEYBOARDKEY::F1"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::F1").data<KEYBOARDKEY::F2 >("KEYBOARDKEY::F2"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::F2").data<KEYBOARDKEY::F3 >("KEYBOARDKEY::F3"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::F3").data<KEYBOARDKEY::F4 >("KEYBOARDKEY::F4"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::F4").data<KEYBOARDKEY::F5 >("KEYBOARDKEY::F5"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::F5").data<KEYBOARDKEY::F6 >("KEYBOARDKEY::F6"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::F6").data<KEYBOARDKEY::F7 >("KEYBOARDKEY::F7"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::F7").data<KEYBOARDKEY::F8 >("KEYBOARDKEY::F8"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::F8").data<KEYBOARDKEY::F9 >("KEYBOARDKEY::F9"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::F9").data<KEYBOARDKEY::F10 >("KEYBOARDKEY::F10"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::F10").data<KEYBOARDKEY::NUMLOCK >("KEYBOARDKEY::NUMLOCK"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUMLOCK").data<KEYBOARDKEY::SCROLL >("KEYBOARDKEY::SCROLL"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::SCROLL").data<KEYBOARDKEY::NUMPAD7 >("KEYBOARDKEY::NUMPAD7"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUMPAD7").data<KEYBOARDKEY::NUMPAD8 >("KEYBOARDKEY::NUMPAD8"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUMPAD8").data<KEYBOARDKEY::NUMPAD9 >("KEYBOARDKEY::NUMPAD9"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUMPAD9").data<KEYBOARDKEY::SUBTRACT >("KEYBOARDKEY::SUBTRACT"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::SUBTRACT").data<KEYBOARDKEY::NUMPAD4 >("KEYBOARDKEY::NUMPAD4"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUMPAD4").data<KEYBOARDKEY::NUMPAD5 >("KEYBOARDKEY::NUMPAD5"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUMPAD5").data<KEYBOARDKEY::NUMPAD6 >("KEYBOARDKEY::NUMPAD6"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUMPAD6").data<KEYBOARDKEY::ADD >("KEYBOARDKEY::ADD"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::ADD").data<KEYBOARDKEY::NUMPAD1 >("KEYBOARDKEY::NUMPAD1"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUMPAD1").data<KEYBOARDKEY::NUMPAD2 >("KEYBOARDKEY::NUMPAD2"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUMPAD2").data<KEYBOARDKEY::NUMPAD3 >("KEYBOARDKEY::NUMPAD3"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUMPAD3").data<KEYBOARDKEY::NUMPAD0 >("KEYBOARDKEY::NUMPAD0"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUMPAD0").data<KEYBOARDKEY::DECIMAL >("KEYBOARDKEY::DECIMAL"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::DECIMAL").data<KEYBOARDKEY::OEM_102 >("KEYBOARDKEY::OEM_102"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::OEM_102").data<KEYBOARDKEY::F11 >("KEYBOARDKEY::F11"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::F11").data<KEYBOARDKEY::F12 >("KEYBOARDKEY::F12"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::F12").data<KEYBOARDKEY::F13 >("KEYBOARDKEY::F13"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::F13").data<KEYBOARDKEY::F14 >("KEYBOARDKEY::F14"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::F14").data<KEYBOARDKEY::F15 >("KEYBOARDKEY::F15"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::F15").data<KEYBOARDKEY::KANA >("KEYBOARDKEY::KANA"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::KANA").data<KEYBOARDKEY::ABNT_C1 >("KEYBOARDKEY::ABNT_C1"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::ABNT_C1").data<KEYBOARDKEY::CONVERT >("KEYBOARDKEY::CONVERT"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::CONVERT").data<KEYBOARDKEY::NOCONVERT >("KEYBOARDKEY::NOCONVERT"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NOCONVERT").data<KEYBOARDKEY::YEN >("KEYBOARDKEY::YEN"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::YEN").data<KEYBOARDKEY::ABNT_C2 >("KEYBOARDKEY::ABNT_C2"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::ABNT_C2").data<KEYBOARDKEY::NUMPADEQUALS >("KEYBOARDKEY::NUMPADEQUALS"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUMPADEQUALS").data<KEYBOARDKEY::PREVTRACK >("KEYBOARDKEY::PREVTRACK"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::PREVTRACK").data<KEYBOARDKEY::AT >("KEYBOARDKEY::AT"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::AT").data<KEYBOARDKEY::COLON >("KEYBOARDKEY::COLON"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::COLON").data<KEYBOARDKEY::UNDERLINE >("KEYBOARDKEY::UNDERLINE"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::UNDERLINE").data<KEYBOARDKEY::KANJI >("KEYBOARDKEY::KANJI"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::KANJI").data<KEYBOARDKEY::STOP >("KEYBOARDKEY::STOP"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::STOP").data<KEYBOARDKEY::AX >("KEYBOARDKEY::AX"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::AX").data<KEYBOARDKEY::UNLABELED >("KEYBOARDKEY::UNLABELED"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::UNLABELED").data<KEYBOARDKEY::NEXTTRACK >("KEYBOARDKEY::NEXTTRACK"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NEXTTRACK").data<KEYBOARDKEY::NUMPADENTER >("KEYBOARDKEY::NUMPADENTER"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUMPADENTER").data<KEYBOARDKEY::RCONTROL >("KEYBOARDKEY::RCONTROL"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::RCONTROL").data<KEYBOARDKEY::MUTE >("KEYBOARDKEY::MUTE"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::MUTE").data<KEYBOARDKEY::CALCULATOR >("KEYBOARDKEY::CALCULATOR"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::CALCULATOR").data<KEYBOARDKEY::PLAYPAUSE >("KEYBOARDKEY::PLAYPAUSE"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::PLAYPAUSE").data<KEYBOARDKEY::MEDIASTOP >("KEYBOARDKEY::MEDIASTOP"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::MEDIASTOP").data<KEYBOARDKEY::VOLUMEDOWN >("KEYBOARDKEY::VOLUMEDOWN"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::VOLUMEDOWN").data<KEYBOARDKEY::VOLUMEUP >("KEYBOARDKEY::VOLUMEUP"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::VOLUMEUP").data<KEYBOARDKEY::WEBHOME >("KEYBOARDKEY::WEBHOME"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::WEBHOME").data<KEYBOARDKEY::NUMPADCOMMA >("KEYBOARDKEY::NUMPADCOMMA"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NUMPADCOMMA").data<KEYBOARDKEY::DIVIDE >("KEYBOARDKEY::DIVIDE"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::DIVIDE").data<KEYBOARDKEY::SYSRQ >("KEYBOARDKEY::SYSRQ"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::SYSRQ").data<KEYBOARDKEY::RMENU >("KEYBOARDKEY::RMENU"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::RMENU").data<KEYBOARDKEY::PAUSE >("KEYBOARDKEY::PAUSE"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::PAUSE").data<KEYBOARDKEY::HOME >("KEYBOARDKEY::HOME"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::HOME").data<KEYBOARDKEY::UP >("KEYBOARDKEY::UP"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::UP").data<KEYBOARDKEY::PRIOR >("KEYBOARDKEY::PRIOR"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::PRIOR").data<KEYBOARDKEY::LEFT >("KEYBOARDKEY::LEFT"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::LEFT").data<KEYBOARDKEY::RIGHT >("KEYBOARDKEY::RIGHT"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::RIGHT").data<KEYBOARDKEY::END >("KEYBOARDKEY::END"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::END").data<KEYBOARDKEY::DOWN >("KEYBOARDKEY::DOWN"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::DOWN").data<KEYBOARDKEY::NEXT >("KEYBOARDKEY::NEXT"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::NEXT").data<KEYBOARDKEY::INSERT >("KEYBOARDKEY::INSERT"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::INSERT").data<KEYBOARDKEY::DEL >("KEYBOARDKEY::DEL"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::DEL").data<KEYBOARDKEY::LWIN >("KEYBOARDKEY::LWIN"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::LWIN").data<KEYBOARDKEY::RWIN >("KEYBOARDKEY::RWIN"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::RWIN").data<KEYBOARDKEY::APPS >("KEYBOARDKEY::APPS"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::APPS").data<KEYBOARDKEY::POWER >("KEYBOARDKEY::POWER"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::POWER").data<KEYBOARDKEY::SLEEP >("KEYBOARDKEY::SLEEP"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::SLEEP").data<KEYBOARDKEY::WAKE >("KEYBOARDKEY::WAKE"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::WAKE").data<KEYBOARDKEY::WEBSEARCH >("KEYBOARDKEY::WEBSEARCH"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::WEBSEARCH").data<KEYBOARDKEY::WEBFAVORITES >("KEYBOARDKEY::WEBFAVORITES"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::WEBFAVORITES").data<KEYBOARDKEY::WEBREFRESH >("KEYBOARDKEY::WEBREFRESH"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::WEBREFRESH").data<KEYBOARDKEY::WEBSTOP >("KEYBOARDKEY::WEBSTOP"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::WEBSTOP").data<KEYBOARDKEY::WEBFORWARD >("KEYBOARDKEY::WEBFORWARD"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::WEBFORWARD").data<KEYBOARDKEY::WEBBACK >("KEYBOARDKEY::WEBBACK"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::WEBBACK").data<KEYBOARDKEY::MYCOMPUTER >("KEYBOARDKEY::MYCOMPUTER"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::MYCOMPUTER").data<KEYBOARDKEY::MAIL >("KEYBOARDKEY::MAIL"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::MAIL").data<KEYBOARDKEY::MEDIASELECT >("KEYBOARDKEY::MEDIASELECT"_hs).prop(Reflection::Prop::Name, "KEYBOARDKEY::MEDIASELECT");
}
void VispredRegister::Register_Components()
{
	META_ADD_COMP(BulletComponent, BulletComponent::SparkParticleName, BulletComponent::BloodSplashParticleName, BulletComponent::Damage, BulletComponent::Speed);
	META_ADD_COMP(PlayerComponent
		, PlayerComponent::IsAttacking
		, PlayerComponent::CurrentFSM
		, PlayerComponent::CameraPosName, PlayerComponent::CameraName, PlayerComponent::FirePosName, PlayerComponent::HandName, PlayerComponent::VPHandName, PlayerComponent::LongswordName
		, PlayerComponent::NonDamageTime, PlayerComponent::DamageReduce
		, PlayerComponent::MaxHP, PlayerComponent::HP
		, PlayerComponent::Sencitive
		, PlayerComponent::SearchDistance
		, PlayerComponent::JumpForce
		, PlayerComponent::WalkSpeed, PlayerComponent::RunSpeed, PlayerComponent::VPRunSpeed
		, PlayerComponent::SlideDuration
		, PlayerComponent::DashDuration
		, PlayerComponent::SlideMultiple
		, PlayerComponent::DashMultiple
		, PlayerComponent::AirControlPercent
		, PlayerComponent::GravityPower
		, PlayerComponent::StaticFriction
		, PlayerComponent::VPGageCoolTime
		, PlayerComponent::TransformationTime
	);
	META_ADD_COMP(InterectiveComponent, InterectiveComponent::IsInterective, InterectiveComponent::SearcehdColor, InterectiveComponent::Soundkey, InterectiveComponent::Volume);
	META_ADD_COMP(PlayerSoundComponent
		, PlayerSoundComponent::Volume_Walk
		, PlayerSoundComponent::Volume_Run
		, PlayerSoundComponent::Volume_Jump
		, PlayerSoundComponent::Volume_Landing
		, PlayerSoundComponent::Volume_Slide
		, PlayerSoundComponent::Volume_Dash
		, PlayerSoundComponent::Volume_Sit
		, PlayerSoundComponent::Volume_Hurt
		, PlayerSoundComponent::Volume_Death
		, PlayerSoundComponent::Volume_GunDraw
		, PlayerSoundComponent::Volume_GunThrow
		, PlayerSoundComponent::Volume_Sword1
		, PlayerSoundComponent::Volume_Sword2
		, PlayerSoundComponent::Volume_Sword3
		, PlayerSoundComponent::Volume_VPAttack1
		, PlayerSoundComponent::Volume_VPAttack2
		, PlayerSoundComponent::Volume_Heal
		, PlayerSoundComponent::Volume_Transformation
		, PlayerSoundComponent::SoundKey_Walk1
		, PlayerSoundComponent::SoundKey_Walk2
		, PlayerSoundComponent::SoundKey_Run1
		, PlayerSoundComponent::SoundKey_Run2
		, PlayerSoundComponent::SoundKey_Jump
		, PlayerSoundComponent::Volume_Landing
		, PlayerSoundComponent::SoundKey_Slide
		, PlayerSoundComponent::SoundKey_Dash
		, PlayerSoundComponent::SoundKey_Sit
		, PlayerSoundComponent::SoundKey_Hurt
		, PlayerSoundComponent::SoundKey_Death
		, PlayerSoundComponent::SoundKey_GunDraw
		, PlayerSoundComponent::SoundKey_GunThrow
		, PlayerSoundComponent::SoundKey_Sword1
		, PlayerSoundComponent::SoundKey_Sword2
		, PlayerSoundComponent::SoundKey_Sword3
		, PlayerSoundComponent::SoundKey_VPAttack1
		, PlayerSoundComponent::SoundKey_VPAttack2
		, PlayerSoundComponent::SoundKey_Heal
		, PlayerSoundComponent::SoundKey_Transformation);
	META_ADD_COMP(TrunComponent, TrunComponent::MoveTime, TrunComponent::Is_X, TrunComponent::Angle, TrunComponent::Finished);
	META_ADD_COMP(AreaAttackComponent, AreaAttackComponent::IdentityAttach, AreaAttackComponent::Damage);
	META_ADD_COMP(PlayerMeleeComponent, PlayerMeleeComponent::DefalutPrefab, PlayerMeleeComponent::DashPrefab, PlayerMeleeComponent::AttackMode, PlayerMeleeComponent::SwordLength, PlayerMeleeComponent::SwordAngle, PlayerMeleeComponent::SwordDamage, PlayerMeleeComponent::VPLength, PlayerMeleeComponent::VPAngle, PlayerMeleeComponent::VPDamage);

	META_ADD_COMP(EnemyComponent
		, EnemyComponent::HP
		, EnemyComponent::CurrentFSM
		, EnemyComponent::CurrentAni
		, EnemyComponent::EnemyType
		, EnemyComponent::HorizontalFOV, EnemyComponent::VerticalFOV
		, EnemyComponent::NearZ, EnemyComponent::FarZ
		, EnemyComponent::IsModelFlipped
		, EnemyComponent::NoiseRangeRadius, EnemyComponent::ChaseRangeRadius
		, EnemyComponent::DeadTime
		, EnemyComponent::ReachableDistanceToPlayer, EnemyComponent::MinRangedAttackRange
		, EnemyComponent::ConsecutiveAttackDelay, EnemyComponent::AttackCycleDelay
		, EnemyComponent::OnHit
		, EnemyComponent::AttackAccuracy
		, EnemyComponent::AccuracyRangeOne, EnemyComponent::AccuracyRangeTwo, EnemyComponent::AccuracyRangeThree, EnemyComponent::AccuracyRangeFour
		, EnemyComponent::AccuracyPenaltyOne, EnemyComponent::AccuracyPenaltyTwo, EnemyComponent::AccuracyPenaltyThree, EnemyComponent::AccuracyPenaltyFour
		, EnemyComponent::AttackPower
		, EnemyComponent::AttackCycleDelay
		, EnemyComponent::ConsecutiveAttackDelay
		, EnemyComponent::MaxShotPerBurst
	);
	META_ADD_COMP(EnemySoundComponent
		, EnemySoundComponent::Volume_Run
		, EnemySoundComponent::Volume_Hurt
		, EnemySoundComponent::Volume_Attack
		, EnemySoundComponent::Volume_Death1
		, EnemySoundComponent::Volume_Death2
		, EnemySoundComponent::Volume_Death3
		, EnemySoundComponent::SoundKey_Run
		, EnemySoundComponent::SoundKey_Hurt
		, EnemySoundComponent::SoundKey_Attack
		, EnemySoundComponent::SoundKey_Death1
		, EnemySoundComponent::SoundKey_Death2
		, EnemySoundComponent::SoundKey_Death3
	)

		META_ADD_COMP(GunComponent, GunComponent::Type, GunComponent::BulletPrefab, GunComponent::MuzzleEffectPointLightPrefab, GunComponent::BulletSpeed, GunComponent::ThrowDamage, GunComponent::Damage1, GunComponent::Damage2, GunComponent::Damage3, GunComponent::BulletSize, GunComponent::ShotGunDistance, GunComponent::SoundKey_GunSound, GunComponent::SoundKey_GunDrop, GunComponent::Volume_GunSound, GunComponent::Volume_GunDrop, GunComponent::CoolTime, GunComponent::CurrentBullet, GunComponent::Bullets, GunComponent::RecoilPos, GunComponent::RecoilMaxXY, GunComponent::RecoilBack, GunComponent::RecoilTime, GunComponent::RecoilPercent);
	META_ADD_COMP(AimUIComponent
		, AimUIComponent::Killed
		, AimUIComponent::Aimed
		, AimUIComponent::Attacked
		, AimUIComponent::Interected
	);
	META_ADD_COMP(ShotGunBulletComponent
		, ShotGunBulletComponent::SparkParticleName
		, ShotGunBulletComponent::BloodSplashParticleName
		, ShotGunBulletComponent::Damage1, ShotGunBulletComponent::Damage2, ShotGunBulletComponent::Damage3
		, ShotGunBulletComponent::Distance
		, ShotGunBulletComponent::Speed
	);
	META_ADD_COMP(VPUIComponent
		, VPUIComponent::ChangeColor
		, VPUIComponent::FullImage
		, VPUIComponent::GageImage
	)

		META_ADD_COMP(DoorComponent, DoorComponent::SoundKey, DoorComponent::Volume, DoorComponent::LeftDoor, DoorComponent::RightDoor, DoorComponent::OpenTime, DoorComponent::IsUseserble, DoorComponent::MoveDistance);
	META_ADD_COMP(SectorClearComponent, SectorClearComponent::Ment, SectorClearComponent::OpenDoorIdentity);
	META_ADD_COMP(DoorOpenerComponent, DoorOpenerComponent::Dummy);
	META_ADD_COMP(WeaponBoxComponent, WeaponBoxComponent::SpawnPrefabs, WeaponBoxComponent::TopMesh, WeaponBoxComponent::OpenAngle, WeaponBoxComponent::OpenTime, WeaponBoxComponent::SpawnOffset, WeaponBoxComponent::SpawnDirection, WeaponBoxComponent::SpawnSpeed);
	META_ADD_COMP(CabinetComponent
		, CabinetComponent::RightDoor, CabinetComponent::LeftDoor, CabinetComponent::OpenAngle, CabinetComponent::OpenTime




	);
	META_ADD_COMP(EntityRemoverComponet, EntityRemoverComponet::temp);
	META_ADD_COMP(IdentityRemoverComponent, IdentityRemoverComponent::RemoveIdentitys);
	META_ADD_COMP(HPReducerComponent, HPReducerComponent::MaxHP, HPReducerComponent::DownHP);
	META_ADD_COMP(AutoPickComponent, AutoPickComponent::IsAuto, AutoPickComponent::PickUps);
	META_ADD_COMP(VPDetectionComponent, VPDetectionComponent::Length, VPDetectionComponent::EnemyColor, VPDetectionComponent::InterectColor);
	META_ADD_COMP(DoOnceComponent, DoOnceComponent::temp);
	META_ADD_COMP(DoOnceResetComponent, DoOnceResetComponent::ResetIdentitys);
	META_ADD_COMP(SpawnerComponent, SpawnerComponent::SpawnPrefab, SpawnerComponent::SpawnTransform);
	META_ADD_COMP(DoorAccessComponent, DoorAccessComponent::Open, DoorAccessComponent::DoorIdentitys);
	META_ADD_COMP(SpawnChildComponent
		, SpawnChildComponent::Prefab1
		, SpawnChildComponent::Prefab1_Pose
		, SpawnChildComponent::Prefab2
		, SpawnChildComponent::Prefab2_Pose
		, SpawnChildComponent::Prefab3
		, SpawnChildComponent::Prefab3_Pose
		, SpawnChildComponent::Prefab4
		, SpawnChildComponent::Prefab4_Pose
		, SpawnChildComponent::Prefab5
		, SpawnChildComponent::Prefab5_Pose
	);
	META_ADD_COMP(PingComponent, PingComponent::IsOn);
	META_ADD_COMP(PingAccessComponent, PingAccessComponent::IsUsed, PingAccessComponent::PingIdentity);
	META_ADD_COMP(PingAccessResetComponent, PingAccessResetComponent::PingIdentity);
	META_ADD_COMP(QuestComponent, QuestComponent::PlayerIdentity, QuestComponent::QuestType, QuestComponent::IsStarted, QuestComponent::IsCleared);
	META_ADD_COMP(MainQuestComponent, MainQuestComponent::QuestSequence
		, MainQuestComponent::Volume_Subquest
		, MainQuestComponent::Volume_Mainquest
		, MainQuestComponent::SounKey_Subquest
		, MainQuestComponent::SounKey_Mainquest

	);
	META_ADD_COMP(PlayerUIComponent, PlayerUIComponent::AimUI, PlayerUIComponent::FadeUI, PlayerUIComponent::HitUI, PlayerUIComponent::HPBackGround, PlayerUIComponent::HPGage, PlayerUIComponent::HPBoarder, PlayerUIComponent::InterectionUI, PlayerUIComponent::Player, PlayerUIComponent::VPeyeUI, PlayerUIComponent::WeaponUI, PlayerUIComponent::GodModeUI);
	META_ADD_COMP(CursorComponent, CursorComponent::ShowCursor, CursorComponent::CursorImage, CursorComponent::CursorScale);
	META_ADD_COMP(HitUIComponent, HitUIComponent::IsHitUIOn, HitUIComponent::ProgressTime, HitUIComponent::DurationTime);
	META_ADD_COMP(FPSComponent, FPSComponent::IsShow);
	META_ADD_COMP(SceneChangeComponent, SceneChangeComponent::ResetButton, SceneChangeComponent::ScenePath, SceneChangeComponent::SceneChangeable);
	META_ADD_COMP(SpawnSoundComponent, SpawnSoundComponent::SoundKey_Volume_2D_Loop);
	META_ADD_COMP(ImageBounceComponent, ImageBounceComponent::AddedBounce, ImageBounceComponent::AddScalePercent, ImageBounceComponent::MaxScalePercent, ImageBounceComponent::BouncingTime, ImageBounceComponent::BounceTimePercent);
	META_ADD_COMP(TextBounceComponent, TextBounceComponent::AddedBounce, TextBounceComponent::AddScalePercent, TextBounceComponent::MaxScalePercent, TextBounceComponent::BouncingTime, TextBounceComponent::BounceTimePercent);
	META_ADD_COMP(FollowComponent, FollowComponent::Isfollowing, FollowComponent::FollowingEntityID);
	META_ADD_COMP(ParticleOwnerComponent, ParticleOwnerComponent::ParticleName);
	META_ADD_COMP(MainTopicComponent
		, MainTopicComponent::BackGroundColor, MainTopicComponent::MentColor
		, MainTopicComponent::Mode
		, MainTopicComponent::IsReset
		, MainTopicComponent::IsShowing
		, MainTopicComponent::Duration
		, MainTopicComponent::Progress
		, MainTopicComponent::Ment);
	META_ADD_COMP(ScoreComponent
		, ScoreComponent::PlayerPoint
		, ScoreComponent::Ment
		, ScoreComponent::EndMent
		, ScoreComponent::FontColor
		, ScoreComponent::EndFontColor
		, ScoreComponent::MiddlePose
		, ScoreComponent::MiddleScale);
	META_ADD_COMP(BestScoreComponent, BestScoreComponent::Score, BestScoreComponent::BestScoreMent, BestScoreComponent::NewBestScoreMent, BestScoreComponent::BasicColor, BestScoreComponent::NewColor);
		META_ADD_COMP(GameBaseComponent, GameBaseComponent::PlayerName, GameBaseComponent::Reduce, GameBaseComponent::MaxHP, GameBaseComponent::MouseSencitive, GameBaseComponent::SencitiveUI, GameBaseComponent::SencitiveEntity, GameBaseComponent::ShowDuration, GameBaseComponent::ShowProgress);
		META_ADD_COMP(AddPrefabButtonComponent, AddPrefabButtonComponent::Button, AddPrefabButtonComponent::Prefabname, AddPrefabButtonComponent::Prefab_Pose);
		META_ADD_COMP(DestroySelfComponent, DestroySelfComponent::Button);
		META_ADD_COMP(DeletePrefabComponent, DeletePrefabComponent::EntityIdentity, DeletePrefabComponent::DestoryAll, DeletePrefabComponent::DestoryOne);

}

void VispredRegister::Register_EnumClass()
{
	using namespace VisPred::Game;
	META_ADD_ENUMCLASS(PlayerFSM
		, PlayerFSM::IDLE
		, PlayerFSM::WALK
		, PlayerFSM::RUN
		, PlayerFSM::CROUCH
		, PlayerFSM::Dash_Slide
		, PlayerFSM::JUMP
		, PlayerFSM::DIE
		, PlayerFSM::DIE_END
		, PlayerFSM::Transformation);
	META_ADD_ENUMCLASS(PlayerMelee, PlayerMelee::Sword_First, PlayerMelee::Sword_Second, PlayerMelee::Sword_Third, PlayerMelee::Sword_Fourth, PlayerMelee::VP_Left, PlayerMelee::VP_Right, PlayerMelee::END);
	META_ADD_ENUMCLASS(GunRecoilMode, GunRecoilMode::ReturnToEndAim, GunRecoilMode::ReturnToMiddle);
	META_ADD_ENUMCLASS(EnemyStates, EnemyStates::Idle, EnemyStates::Chase, EnemyStates::Patrol, EnemyStates::Dead);
	META_ADD_ENUMCLASS(EnemyAni, EnemyAni::ATTACK, EnemyAni::IDLE, EnemyAni::CHASE, EnemyAni::JUMP, EnemyAni::WALK, EnemyAni::DIE, EnemyAni::ATTACKED, EnemyAni::BACKWALK, EnemyAni::ATTACK_IDLE);
	META_ADD_ENUMCLASS(GunType, GunType::NONE, GunType::PISTOL, GunType::RIFLE, GunType::SHOTGUN, GunType::END);
	META_ADD_ENUMCLASS(PlayerAni
		, PlayerAni::ToAttack_Pistol
		, PlayerAni::ToAttack_Rifle
		, PlayerAni::ToAttack_ShotGun
		, PlayerAni::ToIdle01_Pistol
		, PlayerAni::ToIdle01_Rifle
		, PlayerAni::ToIdle01_ShotGun
		, PlayerAni::ToIdle02_Pistol
		, PlayerAni::ToIdle02_Rifle
		, PlayerAni::ToIdle02_ShotGun
		, PlayerAni::Tohook_Sword
		, PlayerAni::Tohook_Pistol
		, PlayerAni::Tohook_Rifle
		, PlayerAni::Tohook_ShotGun
		, PlayerAni::Tointeraction
		, PlayerAni::ToAttack1_Sword
		, PlayerAni::ToAttack2_Sword
		, PlayerAni::ToAttack3_Sword
		, PlayerAni::ToIdle01_Sword
		, PlayerAni::ToIdle02_Sword
		, PlayerAni::ToThrow_Pistol
		, PlayerAni::ToThrow_Rifle
		, PlayerAni::ToThrow_ShotGun
		, PlayerAni::End
	);
	META_ADD_ENUMCLASS(QuestType
		, QuestType::VPMOVE
		, QuestType::VPJUMP
		, QuestType::VPDASH
		, QuestType::VPCHANGE
		, QuestType::PLAYERSHOOT
		, QuestType::PLAYERRUN
		, QuestType::PLAYERJUMP
		, QuestType::PLAYERCROUCH
		, QuestType::PLAYERSLIDE
		, QuestType::PLAYERATTACK
		, QuestType::PLAYERPICKUP
		, QuestType::PLAYERTHROW
		, QuestType::PLAYERINTERECT
		, QuestType::MOUSESENSITIVE);


	META_ADD_ENUMCLASS(TopicType

		, TopicType::FINDBELL
		, TopicType::KILLALL
		, TopicType::NONE
		, TopicType::END
	);



	META_ADD_ENUMCLASS(VPAni
		, VPAni::ToVP_attack_L
		, VPAni::ToVP_attack_R
		, VPAni::ToVP_Idle
		, VPAni::ToVP_dash
		, VPAni::ToVP_jump
		, VPAni::ToVP_run
		, VPAni::ToVP_draw
		, VPAni::End
	);
	Register_KeyBoardKey();














}

void VispredRegister::Register_Structs()
{
	using namespace VPMath;
	using namespace VisPred::Game;

	// std::tuple<std::wstring, float, float> 등록
	entt::meta<std::tuple<std::wstring, float, float>>()
		.type("std::tuple<std::wstring, float, float>"_hs)
		// std::get을 사용하는 대신, tuple의 각 요소에 직접 접근합니다.
		.prop("first"_hs, [](const std::tuple<std::wstring, float, float>& tuple) { return std::get<0>(tuple); })    // 첫 번째 항목 (std::wstring)
		.prop("second"_hs, [](const std::tuple<std::wstring, float, float>& tuple) { return std::get<1>(tuple); })   // 두 번째 항목 (float)
		.prop("third"_hs, [](const std::tuple<std::wstring, float, float>& tuple) { return std::get<2>(tuple); });   // 세 번째 항목 (float)

	// std::vector<std::tuple<std::wstring, float, float>> 등록
	entt::meta<std::vector<std::tuple<std::wstring, float, float>>>()
		.type("std::vector<std::tuple<std::wstring, float, float>>"_hs);

	// Register std::tuple<Vector3, Vector3, Vector3>
	entt::meta<std::tuple<Vector3, Vector3, Vector3>>()
		.type("std::tuple<Vector3, Vector3, Vector3>"_hs)
		.prop("first"_hs, [](const std::tuple<Vector3, Vector3, Vector3>& tuple) { return std::get<0>(tuple); })  // First element (Vector3)
		.prop("second"_hs, [](const std::tuple<Vector3, Vector3, Vector3>& tuple) { return std::get<1>(tuple); }) // Second element (Vector3)
		.prop("third"_hs, [](const std::tuple<Vector3, Vector3, Vector3>& tuple) { return std::get<2>(tuple); }); // Third element (Vector3)

	// Register std::tuple<std::string, int, bool, bool>
	entt::meta<std::tuple<std::string, int, bool, bool>>()
		.type("std::tuple<std::string, int, bool, bool>"_hs)
		.prop("first"_hs, [](const std::tuple<std::string, int, bool, bool>& tuple) { return std::get<0>(tuple); })  // First element (std::string)
		.prop("second"_hs, [](const std::tuple<std::string, int, bool, bool>& tuple) { return std::get<1>(tuple); }) // Second element (int)
		.prop("third"_hs, [](const std::tuple<std::string, int, bool, bool>& tuple) { return std::get<2>(tuple); })  // Third element (bool)
		.prop("fourth"_hs, [](const std::tuple<std::string, int, bool, bool>& tuple) { return std::get<3>(tuple); }); // Fourth element (bool)

	entt::meta < std::array < std::wstring, (int)TopicType::END >>().type("std::array<std::wstring,(int)TopicType::END>"_hs);

}

void VispredRegister::Register_Value()
{
}

void VispredRegister::Register_VPMath()
{
}
