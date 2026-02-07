cmake_minimum_required(VERSION 3.22)
project(VelyraCore DESCRIPTION "Velyra Core components (like window manager and context manager)")

include(VelyraBuildUtils/VelyraBuildUtils.cmake)
include(cmake/Glad.cmake)
include(cmake/ImLib.cmake)

set(VELYRA_UTILS_PATH)
include_or_fetch(VelyraUtils VELYRA_UTILS_PATH)
message(STATUS "VelyraUtils found at ${VELYRA_UTILS_PATH}")

set(VELYRA_IMAGE_PATH)
include_or_fetch(VelyraImage VELYRA_IMAGE_PATH)
message(STATUS "VelyraImage found at ${VELYRA_IMAGE_PATH}")

set(VELYRA_CORE_LIBS)

set(VELYRA_CORE_PLATFORM_HEADERS)
set(VELYRA_CORE_PLATFORM_SRC)

set(VELYRA_CORE_PLATFORM_TEST_SRC)

if (WIN32)
    include(cmake/VelyraCoreWin32.cmake)
elseif (UNIX)
    include(cmake/VelyraCoreUnix.cmake)
endif ()

set(VELYRA_CORE_LOGGING_HEADERS
    src/VelyraCore/Logging/LoggerNames.hpp
    src/VelyraCore/Logging/GLLogging.hpp
)

set(VELYRA_CORE_LOGGING_SRC
    src/VelyraCore/Logging/GLLogging.cpp
)

set(VELYRA_CORE_WINDOW_HEADERS
    include/VelyraCore/Window/Window.hpp
    include/VelyraCore/Window/Event.hpp
    include/VelyraCore/Window/KeyboardButtons.hpp
    include/VelyraCore/Window/MouseButtons.hpp
    include/VelyraCore/Window/WindowFactory.hpp
)

set(VELYRA_CORE_WINDOW_SRC
    src/VelyraCore/Window/WindowFactory.cpp
)

set(VELYRA_CORE_CONTEXT_HEADERS
    include/VelyraCore/Context/Definitions.hpp
    include/VelyraCore/Context/Device.hpp
    include/VelyraCore/Context/Context.hpp
    include/VelyraCore/Context/Viewport.hpp
    include/VelyraCore/Context/ShaderModule.hpp
    include/VelyraCore/Context/Shader.hpp
    include/VelyraCore/Context/VertexLayout.hpp
    include/VelyraCore/Context/VertexBuffer.hpp
    include/VelyraCore/Context/IndexBuffer.hpp
    include/VelyraCore/Context/MeshBinding.hpp
    include/VelyraCore/Context/ConstantBuffer.hpp

    src/VelyraCore/Context/OpenGL/GLContext.hpp
    src/VelyraCore/Context/OpenGL/GLDevice.hpp
    src/VelyraCore/Context/OpenGL/GLViewport.hpp
    src/VelyraCore/Context/OpenGL/GLShaderModule.hpp
    src/VelyraCore/Context/OpenGL/GLShader.hpp
    src/VelyraCore/Context/OpenGL/GLVertexBuffer.hpp
    src/VelyraCore/Context/OpenGL/GLIndexBuffer.hpp
    src/VelyraCore/Context/OpenGL/GLMeshBinding.hpp
    src/VelyraCore/Context/OpenGL/GLConstantBuffer.hpp

    src/VelyraCore/Context/OpenGL/Internal/GLPlatformContext.hpp
    src/VelyraCore/Context/OpenGL/Internal/GLTranslations.hpp
    src/VelyraCore/Context/OpenGL/Internal/GLBuffer.hpp
)

set(VELYRA_CORE_CONTEXT_SRC
    src/VelyraCore/Context/Context.cpp
    src/VelyraCore/Context/VertexLayout.cpp

    src/VelyraCore/Context/OpenGL/GLContext.cpp
    src/VelyraCore/Context/OpenGL/GLDevice.cpp
    src/VelyraCore/Context/OpenGL/GLViewport.cpp
    src/VelyraCore/Context/OpenGL/GLShaderModule.cpp
    src/VelyraCore/Context/OpenGL/GLShader.cpp
    src/VelyraCore/Context/OpenGL/GLVertexBuffer.cpp
    src/VelyraCore/Context/OpenGL/GLIndexBuffer.cpp
    src/VelyraCore/Context/OpenGL/GLMeshBinding.cpp
    src/VelyraCore/Context/OpenGL/GLConstantBuffer.cpp

    src/VelyraCore/Context/OpenGL/Internal/GLTranslations.cpp
    src/VelyraCore/Context/OpenGL/Internal/GLBuffer.cpp
)

set(VELYRA_CORE_HEADERS
    ${VELYRA_CORE_PLATFORM_HEADERS}
    ${VELYRA_CORE_LOGGING_HEADERS}
    ${VELYRA_CORE_WINDOW_HEADERS}
    ${VELYRA_CORE_CONTEXT_HEADERS}

    include/VelyraCore/VelyraCore.hpp

    src/VelyraCore/Pch.hpp
)

set(VELYRA_CORE_SRC
    ${VELYRA_CORE_PLATFORM_SRC}
    ${VELYRA_CORE_LOGGING_SRC}
    ${VELYRA_CORE_WINDOW_SRC}
    ${VELYRA_CORE_CONTEXT_SRC}

    src/VelyraCore/VelyraCore.cpp
)

add_library(VelyraCore STATIC ${VELYRA_CORE_HEADERS} ${VELYRA_CORE_SRC})
velyra_target_set_compile_flags(VelyraCore)
target_precompile_headers(VelyraCore PRIVATE src/VelyraCore/Pch.hpp)
target_include_directories(VelyraCore PUBLIC include)
target_include_directories(VelyraCore PRIVATE include/VelyraCore/ImGui)
target_link_libraries(VelyraCore PUBLIC VelyraUtils VelyraImage ImLib)
target_link_libraries(VelyraCore PRIVATE ${VELYRA_CORE_LIBS})
target_compile_definitions(VelyraCore PRIVATE
        $<$<CONFIG:Debug>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE>
        $<$<NOT:$<CONFIG:Debug>>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO>
)

################# Sandbox #################

set(VELYRA_CORE_SANDBOX_SRC
    src/VelyraCoreSandBox/SandBox.cpp
    src/VelyraCoreSandBox/SandBox.hpp
    src/VelyraCoreSandBox/SandBoxInclude.hpp
    src/VelyraCoreSandBox/DataTypes.hpp
    src/VelyraCoreSandBox/IComponent.hpp
    src/VelyraCoreSandBox/IProcedure.hpp
    src/VelyraCoreSandBox/ProcedureExecutor.cpp
    src/VelyraCoreSandBox/ProcedureExecutor.hpp
    src/VelyraCoreSandBox/Utils.cpp
    src/VelyraCoreSandBox/Utils.hpp
    src/VelyraCoreSandBox/ImGuiWidgets.cpp
    src/VelyraCoreSandBox/ImGuiWidgets.hpp

    src/VelyraCoreSandBox/Procedures/EventProcedure.cpp
    src/VelyraCoreSandBox/Procedures/EventProcedure.hpp
    src/VelyraCoreSandBox/Procedures/ImGuiProcedure.cpp
    src/VelyraCoreSandBox/Procedures/ImGuiProcedure.hpp
    src/VelyraCoreSandBox/Procedures/ContextInfoProcedure.cpp
    src/VelyraCoreSandBox/Procedures/ContextInfoProcedure.hpp
    src/VelyraCoreSandBox/Procedures/ClearDefaultFrameBufferProcedure.hpp
    src/VelyraCoreSandBox/Procedures/ClearDefaultFrameBufferProcedure.cpp
    src/VelyraCoreSandBox/Procedures/FileDialogProcedure.hpp
    src/VelyraCoreSandBox/Procedures/FileDialogProcedure.cpp
    src/VelyraCoreSandBox/Procedures/VertexDrawProcedure.hpp
    src/VelyraCoreSandBox/Procedures/VertexDrawProcedure.cpp
    src/VelyraCoreSandBox/Procedures/IndexDrawProcedure.cpp
    src/VelyraCoreSandBox/Procedures/IndexDrawProcedure.hpp
    src/VelyraCoreSandBox/Procedures/CameraProcedure.cpp
    src/VelyraCoreSandBox/Procedures/CameraProcedure.hpp
    src/VelyraCoreSandBox/Procedures/ProjectionProcedure.cpp
    src/VelyraCoreSandBox/Procedures/ProjectionProcedure.hpp
    src/VelyraCoreSandBox/Procedures/ModelViewProjectionProcedure.cpp
    src/VelyraCoreSandBox/Procedures/ModelViewProjectionProcedure.hpp
)

if (BUILD_SANDBOX)
    fetch_glm()

    add_executable(VelyraCoreSandBox main.cpp ${VELYRA_CORE_SANDBOX_SRC})
    target_link_libraries(VelyraCoreSandBox PRIVATE VelyraCore glm::glm)
    target_compile_definitions(VelyraCoreSandBox PRIVATE
            $<$<CONFIG:Debug>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE>
            $<$<NOT:$<CONFIG:Debug>>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO>
    )

endif ()

################# Testing #################

set(VELYRA_CORE_TEST_SRC
    ${VELYRA_CORE_PLATFORM_TEST_SRC}

    test/Window/TestWindow.cpp

    test/Context/TestContext.cpp
    test/Context/TestShaderModule.cpp
    test/Context/TestShader.cpp
    test/Context/TestVertexLayout.cpp
    test/Context/TestVertexBuffer.cpp
    test/Context/TestIndexBuffer.cpp
    test/Context/TestConstantBuffer.cpp

    test/Context/OpenGL/TestGLTranslations.cpp
)

if (BUILD_TESTING)
    fetch_gtest()

    add_executable(TestVelyraCore ${VELYRA_CORE_TEST_SRC})
    target_include_directories(TestVelyraCore PUBLIC include)
    target_include_directories(TestVelyraCore PRIVATE include/VelyraCore/ImGui)
    target_precompile_headers(TestVelyraCore PUBLIC test/TestPch.hpp)
    target_link_libraries(TestVelyraCore PUBLIC ${VELYRA_CORE_LIBS} GTest::gtest GTest::gtest_main VelyraCore)
    add_test(NAME CTestVelyraCore COMMAND ${PROOT} $<TARGET_FILE:TestVelyraCore>)
    set_tests_properties(CTestVelyraCore PROPERTIES
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif ()

