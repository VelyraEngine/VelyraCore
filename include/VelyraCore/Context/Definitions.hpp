#pragma once

#include <VelyraCore/Core/CoreInclude.hpp>
#include <VelyraUtils/VelyraEnum.hpp>

VL_ENUM(VL_GRAPHICS_API, int,
    VL_API_NONE,
    VL_API_BEST,
    VL_API_OPENGL,
    VL_API_DIRECT3D11
);

#define VL_GL_MAJOR_VERSION 4
#define VL_GL_MINOR_VERSION 6