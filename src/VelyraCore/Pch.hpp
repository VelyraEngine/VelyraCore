#pragma once

#include <filesystem>

#include <VelyraUtils/VelyraUtils.hpp>

#if defined(VL_PLATFORM_WINDOWS)

    #if !defined(NOMINMAX)
        #define NOMINMAX
    #endif

    #include <windows.h>
    #include <dshow.h>
    #include <commdlg.h>
    #include <comdef.h>

    #include <d3d11.h>
    #include <dxgi.h>
    #include <d3dcompiler.h>
    #include <dxgiformat.h>
    #include <dxgidebug.h>

#endif
