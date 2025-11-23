#pragma once

#include <VelyraUtils/VelyraEnum.hpp>

VL_ENUM(VL_GRAPHICS_API, int,
    VL_GRAPHICS_API_NONE    = 0x00,
    VL_GRAPHICS_API_OPENGL  = 0x01,
    VL_GRAPHICS_API_VULKAN  = 0x10,
    VL_GRAPHICS_API_D3D11   = 0x08,
    VL_GRAPHICS_API_D3D12   = 0x09
    );

