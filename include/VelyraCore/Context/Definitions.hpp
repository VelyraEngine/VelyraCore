#pragma once

#include <VelyraCore/Core/CoreInclude.hpp>
#include <VelyraUtils/VelyraEnum.hpp>

#define VL_GL_MAJOR_VERSION 4
#define VL_GL_MINOR_VERSION 6

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

VL_ENUM(VL_SHADER_TYPE, int,
    VL_SHADER_NONE                   = 0x00,
    VL_SHADER_VERTEX                 = 0x01,
    VL_SHADER_FRAGMENT               = 0x02,
    VL_SHADER_PIXEL                  = 0x02,
    VL_SHADER_GEOMETRY               = 0x03,
    VL_SHADER_TESSELATION_EVALUATION = 0x04,
    VL_SHADER_TESSELATION_CONTROL    = 0x05,
    VL_SHADER_COMPUTE                = 0x06
);

VL_ENUM(VL_BUFFER_USAGE, int,
    VL_BUFFER_USAGE_DEFAULT       = 0x00,   // GPU read/write
    VL_BUFFER_USAGE_DYNAMIC       = 0x01,   // CPU write, GPU read
    VL_BUFFER_USAGE_STATIC        = 0x02   // GPU read, CPU write only when initializing
);

VL_ENUM(VL_DRAW_MODE, int,
    VL_DRAW_NONE            = 0x00,
    VL_DRAW_POINTS          = 0x01,
    VL_DRAW_LINES           = 0x02,
    VL_DRAW_LINES_STRIP     = 0x03,
    VL_DRAW_TRIANGLES       = 0x04,
    VL_DRAW_TRIANGLE_STRIP  = 0x05
);