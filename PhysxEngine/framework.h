#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <memory>
#include <vector>
#include <any>
#include <functional>
#include <malloc.h>
#include <tchar.h>
#include <string_view>
#include <entt/entt.hpp>
#include <d3d11.h>
#include "../physx/PxPhysicsAPI.h"

#pragma comment(lib, "PhysXFoundation_64.lib")
#pragma comment(lib, "PhysXCommon_64.lib")
#pragma comment(lib, "PhysXTask_static_64.lib")
#pragma comment(lib, "PhysX_64.lib")
#pragma comment(lib, "PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysXCooking_64.lib")
#pragma comment(lib, "PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "PhysXExtensions_static_64.lib")

#include "VisPredMath.h"
namespace VPMath = VisPred::SimpleMath;

using namespace entt::literals;
