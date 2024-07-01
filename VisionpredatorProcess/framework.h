#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#include <SDKDDKVer.h>
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
#include <d3d11.h>
#include "../include/entt/entt.hpp"
#include "../VPEngine/VisPredMath.h"
#include "../include/directxtk/SimpleMath.h"
namespace VPMath = VisPred::SimpleMath;
using namespace entt::literals;
#include <crtdbg.h>