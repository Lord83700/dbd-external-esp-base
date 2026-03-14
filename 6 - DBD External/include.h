#pragma once
#include <iostream>
#include <Windows.h>
#include <cstdint>
#include <TlHelp32.h>
#include <string.h>
#include <dwmapi.h>
#include <d3d11.h>
#include <d3d9types.h>
#include <dxgi.h>
#include <vector>
#include <mutex>
#include <thread>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwmapi.lib")

//Driver
#include "Header/Driver/driver.h"
#include "Header/SDK/sdk.h"



//ImGui
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"

//Structure
#include "Header/Struct/struct.h"

//Class Header
#include "Init/init.h"
#include "Overlay/overlay.h"
#include "render/render.h"


//Generic Header
#include "Header/Global/global.h"
#include "Header/Global/config.h"
#include "Header/Global/offsets.h"
#include "Header/Global/bones.h"
#include "Header/Global/bone_mapping.h"

//Cheat
#include "Cheat/cheat.h"
#include "Cheat/Esp/esp.h"
