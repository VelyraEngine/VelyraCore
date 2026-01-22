#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ranges>

#include <VelyraUtils/VelyraUtils.hpp>

#include <VelyraCore/ImGui/imgui.h>
#include <VelyraCore/ImGui/implot.h>
#include "ImGui/imgui_impl_opengl3.h"

#include <glad/glad.h>

#if defined(VL_PLATFORM_WINDOWS)

    #if !defined(NOMINMAX)
        #define NOMINMAX
    #endif

    #include <windows.h>
    #include <dshow.h>
    #include <commdlg.h>
    #include <comdef.h>

    #include <glad/glad_wgl.h>

    #include <d3d11.h>
    #include <dxgi.h>
    #include <d3dcompiler.h>
    #include <dxgiformat.h>
    #include <dxgidebug.h>

    #include "ImGui/imgui_impl_win32.h"

#elif defined(VL_PLATFORM_LINUX)

    #define GLFW_INCLUDE_NONE
    #include <GLFW/glfw3.h>

    #include "ImGui/imgui_impl_glfw.h"

    #include <tinyfiledialogs.h>
#endif
