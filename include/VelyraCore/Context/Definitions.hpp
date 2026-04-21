#pragma once

#include <VelyraCore/Core/CoreInclude.hpp>
#include <VelyraUtils/VelyraEnum.hpp>
#include <VelyraImage/IImage.hpp>

#define VL_GL_MAJOR_VERSION 4
#define VL_GL_MINOR_VERSION 6

VL_ENUM(VL_GRAPHICS_API, int,
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
    VL_DRAW_POINTS          = 0x01,
    VL_DRAW_LINES           = 0x02,
    VL_DRAW_LINES_STRIP     = 0x03,
    VL_DRAW_TRIANGLES       = 0x04,
    VL_DRAW_TRIANGLE_STRIP  = 0x05
);

// The upper 4 bits represent the channel count, the lower 4 bits the data type (same as VL_TYPE)
VL_ENUM(VL_TEXTURE_FORMAT, U8,
    // single channel
    VL_TEXTURE_R_U8     = 0x12,
    VL_TEXTURE_R_I8     = 0x13,
    VL_TEXTURE_R_U16    = 0x14,
    VL_TEXTURE_R_I16    = 0x15,
    VL_TEXTURE_R_U32    = 0x16,
    VL_TEXTURE_R_I32    = 0x17,
    VL_TEXTURE_R_F16    = 0x1A,
    VL_TEXTURE_R_F32    = 0x1B,

    // two channels
    VL_TEXTURE_RG_U8    = 0x22,
    VL_TEXTURE_RG_I8    = 0x23,
    VL_TEXTURE_RG_U16   = 0x24,
    VL_TEXTURE_RG_I16   = 0x25,
    VL_TEXTURE_RG_U32   = 0x26,
    VL_TEXTURE_RG_I32   = 0x27,
    VL_TEXTURE_RG_F16   = 0x2A,
    VL_TEXTURE_RG_F32   = 0x2B,

    // three channels
    VL_TEXTURE_RGB_U8   = 0x32,
    VL_TEXTURE_RGB_I8   = 0x33,
    VL_TEXTURE_RGB_U16  = 0x34,
    VL_TEXTURE_RGB_I16  = 0x35,
    VL_TEXTURE_RGB_U32  = 0x36,
    VL_TEXTURE_RGB_I32  = 0x37,
    VL_TEXTURE_RGB_F16  = 0x3A,
    VL_TEXTURE_RGB_F32  = 0x3B,

    // four channels
    VL_TEXTURE_RGBA_U8  = 0x42,
    VL_TEXTURE_RGBA_I8  = 0x43,
    VL_TEXTURE_RGBA_U16 = 0x44,
    VL_TEXTURE_RGBA_I16 = 0x45,
    VL_TEXTURE_RGBA_U32 = 0x46,
    VL_TEXTURE_RGBA_I32 = 0x47,
    VL_TEXTURE_RGBA_F16 = 0x4A,
    VL_TEXTURE_RGBA_F32 = 0x4B,

    // special formats
    VL_TEXTURE_DEPTH_16     = 0x51,
    VL_TEXTURE_DEPTH_24     = 0x52,
    VL_TEXTURE_DEPTH_32     = 0x53,
    VL_TEXTURE_DEPTH_24_STENCIL_8    = 0x54,
    VL_TEXTURE_DEPTH_32_STENCIL_8    = 0x55
);

VL_ENUM(VL_TEXTURE_FILTER, int,
    VL_TEXTURE_FILTER_NONE      = 0x00,
    VL_TEXTURE_FILTER_LINEAR    = 0x01,
    VL_TEXTURE_FILTER_POINT     = 0x02
);

VL_ENUM(VL_TEXTURE_WRAP, int,
    VL_TEXTURE_WRAP_NONE            = 0x00,
    VL_TEXTURE_WRAP_REPEAT          = 0x01,
    VL_TEXTURE_WRAP_MIRROR_REPEAT   = 0x02,
    VL_TEXTURE_WRAP_CLAMP_EDGE      = 0x03,
    VL_TEXTURE_WRAP_CLAMP_BORDER    = 0x04
);

VL_ENUM(VL_COMPARISON_FUNC, int,
    VL_COMPARISON_FUNC_NEVER        = 0x00,
    VL_COMPARISON_FUNC_LESS         = 0x01,
    VL_COMPARISON_FUNC_EQUAL        = 0x02,
    VL_COMPARISON_FUNC_LESS_EQUAL   = 0x03,
    VL_COMPARISON_FUNC_GREATER      = 0x04,
    VL_COMPARISON_FUNC_NOT_EQUAL    = 0x05,
    VL_COMPARISON_FUNC_GREATER_EQUAL= 0x06,
    VL_COMPARISON_FUNC_ALWAYS       = 0x07
);

VL_ENUM(VL_DEPTH_MASK, int,
    VL_DEPTH_MASK_DISABLED  = 0x00,
    VL_DEPTH_MASK_ENABLED   = 0x01
);

VL_ENUM(VL_STENCIL_FUNC, int,
    VL_STENCIL_FUNC_KEEP             = 0x00,
    VL_STENCIL_FUNC_ZERO             = 0x01,
    VL_STENCIL_FUNC_REPLACE          = 0x02,
    VL_STENCIL_FUNC_INCR             = 0x03,
    VL_STENCIL_FUNC_INCR_WRAP        = 0x04,
    VL_STENCIL_FUNC_DECR             = 0x05,
    VL_STENCIL_FUNC_DECR_WRAP        = 0x06,
    VL_STENCIL_FUNC_INVERT           = 0x07
);

namespace Velyra::Core {

    VL_API VL_TEXTURE_FORMAT getTextureFormat(const View<Image::IImage>& image);

    VL_API VL_TYPE getTextureDataType(VL_TEXTURE_FORMAT format);

    VL_API VL_CHANNEL_FORMAT getTextureChannelFormat(VL_TEXTURE_FORMAT format);

}