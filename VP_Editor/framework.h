﻿#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

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
#include "VisPredMath.h"
#include "../include/directxtk/SimpleMath.h"
namespace VPMath = VisPred::SimpleMath;
using namespace entt::literals;
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "ImGuizmo.h"
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
