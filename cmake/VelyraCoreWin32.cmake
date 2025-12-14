if (MINGW)
    set(VELYRA_CORE_LIBS -limm32 -ldwmapi -luser32 -ldxgi -lopenGL32 -ld3d11 -ld3dcompiler_47 glad)

elseif(MSVC)
    set(VELYRA_CORE_LIBS imm32 dwmapi user32 dxgi dxguid openGL32 d3d11 d3dcompiler glad)
else ()
    message(FATAL_ERROR "Unsupported compiler for WIN32!")
endif()

set(VELYRA_CORE_PLATFORM_HEADERS
    src/VelyraCore/Window/Win32/Win32ForwardDeclarations.hpp
    src/VelyraCore/Window/Win32/WindowWin32.hpp
    src/VelyraCore/Window/Win32/Win32Utils.hpp

    src/VelyraCore/Logging/Win32Logging.hpp

    src/VelyraCore/Context/OpenGL/Internal/WglPlatformContext.hpp
)

set(VELYRA_CORE_PLATFORM_SRC
    src/VelyraCore/Window/Win32/WindowWin32.cpp
    src/VelyraCore/Window/Win32/Win32Utils.cpp

    src/VelyraCore/Logging/Win32Logging.cpp

    src/VelyraCore/Context/OpenGL/Internal/WglPlatformContext.cpp
)

set(VELYRA_CORE_PLATFORM_TEST_SRC
    test/Window/Win32/TestWin32Utils.cpp
)