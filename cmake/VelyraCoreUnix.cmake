include(FetchContent)

set(NFD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(NFD_BUILD_SDL2_TESTS OFF CACHE BOOL "" FORCE)
set(NFD_BUILD_SDL3_TESTS OFF CACHE BOOL "" FORCE)
set(NFD_BUILD_GLFW3_TESTS OFF CACHE BOOL "" FORCE)
set(NFD_INSTALL OFF CACHE BOOL "" FORCE)
option(NFD_PORTAL "Use xdg-desktop-portal for native file dialogs" ON)
set(NFD_X11 ON CACHE BOOL "" FORCE)
set(NFD_WAYLAND ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    nfd
    GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
    GIT_TAG v1.4.1
    GIT_SHALLOW TRUE
    GIT_SUBMODULES_RECURSE TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(nfd)

vl_fetch_glfw()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(VELYRA_CORE_LIBS glad glfw nfd)
else ()
    message(FATAL_ERROR "Unsupported compiler for UNIX!")
endif()

set(VELYRA_CORE_PLATFORM_HEADERS
    src/VelyraCore/Window/Glfw3/Glfw3Utils.hpp
    src/VelyraCore/Window/Glfw3/Glfw3Window.hpp

    src/VelyraCore/Context/OpenGL/Internal/Glfw3PlatformContext.hpp
)

set(VELYRA_CORE_PLATFORM_SRC
    src/VelyraCore/Window/Glfw3/Glfw3Utils.cpp
    src/VelyraCore/Window/Glfw3/Glfw3Window.cpp

    src/VelyraCore/Context/OpenGL/Internal/Glfw3PlatformContext.cpp
)

set(VELYRA_CORE_PLATFORM_TEST_SRC

)