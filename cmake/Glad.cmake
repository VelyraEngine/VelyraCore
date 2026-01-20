if (WIN32)
    set(GLAD_PLATFORM_SRC
        src/VelyraCore/Dependencies/glad/src/glad_wgl.c
    )
elseif (UNIX)
    # we could add glx later, but with the whole wayland situation, let us for now use the GLFW3 one
    set(GLAD_PLATFORM_SRC

    )
endif ()

add_library(glad STATIC src/VelyraCore/Dependencies/glad/src/glad.c ${GLAD_PLATFORM_SRC})
target_include_directories(glad PUBLIC src/VelyraCore/Dependencies/glad/include/)
set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)