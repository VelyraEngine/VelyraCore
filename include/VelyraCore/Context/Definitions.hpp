#pragma once

#include <VelyraCore/Core/CoreInclude.hpp>
#include <VelyraUtils/VelyraEnum.hpp>

VL_ENUM(VL_GRAPHICS_API, int,
    VL_API_NONE,
    VL_API_BEST,
    VL_API_OPENGL,
    VL_API_DIRECT3D11
);

VL_ENUM(VL_IMGUI_STYLE, int,
    VL_IMGUI_STYLE_DEFAULT = 0x00,
    VL_IMGUI_STYLE_CLASSIC = 0x01,
    VL_IMGUI_STYLE_DARK    = 0x02,
    VL_IMGUI_STYLE_LIGHT   = 0x03
);

#define VL_GL_MAJOR_VERSION 4
#define VL_GL_MINOR_VERSION 6