#pragma once

#include "SandBoxInclude.hpp"
#include <VelyraUtils/VelyraEnum.hpp>

VL_ENUM(VL_SBX_PROCEDURE_TYPE, int,
    Unknown = 0,
    EventPrinter,
    ImGuiDemo,
    ContextInfo,
    FileDialog,
    VertexDraw,
    IndexDraw,
    Camera,
    Projection,
    MVP,
    MeshBinding,
    Sampler,
    Texture2D,
    TexturedMVP,
    DepthStencilState,
    FrameBuffer
);
