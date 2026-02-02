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

// The upper 4 bits represent the channel count, the lower 4 bits the data type (same as VL_TYPE)
VL_ENUM(VL_TEXTURE_FORMAT, U8,
    VL_TEXTURE_NONE         = 0x00,

    // single channel
    VL_TEXTURE_R_UI8        = 0x12,
    VL_TEXTURE_R_I8         = 0x13,
    VL_TEXTURE_R_UI16       = 0x14,
    VL_TEXTURE_R_I16        = 0x15,
    VL_TEXTURE_R_UI32       = 0x16,
    VL_TEXTURE_R_I32        = 0x17,
    VL_TEXTURE_R_F16        = 0x1A,
    VL_TEXTURE_R_F32        = 0x1B,

    // two channels
    VL_TEXTURE_RG_UI8       = 0x22,
    VL_TEXTURE_RG_I8        = 0x23,
    VL_TEXTURE_RG_UI16      = 0x24,
    VL_TEXTURE_RG_I16       = 0x25,
    VL_TEXTURE_RG_UI32      = 0x26,
    VL_TEXTURE_RG_I32       = 0x27,
    VL_TEXTURE_RG_F16       = 0x2A,
    VL_TEXTURE_RG_F32       = 0x2B,

    // three channels
    VL_TEXTURE_RGB_UI8      = 0x32,
    VL_TEXTURE_RGB_I8       = 0x33,
    VL_TEXTURE_RGB_UI16     = 0x34,
    VL_TEXTURE_RGB_I16      = 0x35,
    VL_TEXTURE_RGB_UI32     = 0x36,
    VL_TEXTURE_RGB_I32      = 0x37,
    VL_TEXTURE_RGB_F16      = 0x3A,
    VL_TEXTURE_RGB_F32      = 0x3B,

    // four channels
    VL_TEXTURE_RGBA_UI8     = 0x42,
    VL_TEXTURE_RGBA_I8      = 0x43,
    VL_TEXTURE_RGBA_UI16    = 0x44,
    VL_TEXTURE_RGBA_I16     = 0x45,
    VL_TEXTURE_RGBA_UI32    = 0x46,
    VL_TEXTURE_RGBA_I32     = 0x47,
    VL_TEXTURE_RGBA_F16     = 0x4A,
    VL_TEXTURE_RGBA_F32     = 0x4B,

    // special formats
    VL_TEXTURE_DEPTH_16     = 0x51,
    VL_TEXTURE_DEPTH_24     = 0x52,
    VL_TEXTURE_DEPTH_32     = 0x53,
    VL_TEXTURE_DEPTH_24_STENCIL_8    = 0x54,
    VL_TEXTURE_DEPTH_32_STENCIL_8    = 0x55
);

namespace Velyra::Core {

}