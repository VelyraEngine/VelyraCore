# Add tinyfiledialogs library from local Dependencies folder
get_filename_component(VELYRA_CORE_DIR "${CMAKE_CURRENT_LIST_DIR}" DIRECTORY)
add_subdirectory(${VELYRA_CORE_DIR}/Dependencies/Unix/tinyfiledialogs)

vl_fetch_glfw()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(VELYRA_CORE_LIBS glad glfw tinyfiledialogs)
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