#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����
#include <windows.h>
// C ��Ÿ�� ��� �����Դϴ�.
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
#include <PxPhysicsAPI.h>

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

#include "../include/Macro/ReflectionMacro.h"
#include "../include/Macro/VisPredMacro.h"
using namespace physx;