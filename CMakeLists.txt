cmake_minimum_required(VERSION 3.22)
project(VelyraCore DESCRIPTION "Velyra Core components (like window manager and context manager)")

include(VelyraBuildUtils/VelyraBuildUtils.cmake)

set(VELYRA_UTILS_PATH)
include_or_fetch(VelyraUtils VELYRA_UTILS_PATH)
message(STATUS "VelyraUtils found at ${VELYRA_UTILS_PATH}")

set(VELYRA_CORE_LIBS)

set(VELYRA_CORE_PLATFORM_HEADERS)
set(VELYRA_CORE_PLATFORM_SRC)

set(VELYRA_CORE_PLATFORM_TEST_SRC)

if (WIN32)
    include(cmake/VelyraCoreWin32.cmake)
elseif (UNIX)
    message(WARNING "Please add UNIX configuration")
endif ()

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

set(VELYRA_CORE_HEADERS
    ${VELYRA_CORE_PLATFORM_HEADERS}
    ${VELYRA_CORE_WINDOW_HEADERS}

    include/VelyraCore/VelyraCore.hpp

    include/VelyraCore/Core/GraphicsAPI.hpp
    src/VelyraCore/Pch.hpp
    src/VelyraCore/LoggerNames.hpp
)

set(VELYRA_CORE_SRC
    ${VELYRA_CORE_PLATFORM_SRC}
    ${VELYRA_CORE_WINDOW_SRC}

    src/VelyraCore/VelyraCore.cpp
)

add_library(VelyraCore STATIC ${VELYRA_CORE_HEADERS} ${VELYRA_CORE_SRC})
velyra_target_set_compile_flags(VelyraCore)
target_precompile_headers(VelyraCore PRIVATE src/VelyraCore/Pch.hpp)
target_include_directories(VelyraCore PUBLIC include)
target_link_libraries(VelyraCore PUBLIC VelyraUtils)
target_link_libraries(VelyraCore PRIVATE ${VELYRA_CORE_LIBS})

add_executable(VelyraCoreMain main.cpp)
target_link_libraries(VelyraCoreMain PRIVATE VelyraCore)

################# Testing #################

set(VELYRA_CORE_TEST_SRC
    ${VELYRA_CORE_PLATFORM_TEST_SRC}

    test/Window/TestWindow.cpp
)

if (BUILD_TESTING)
    fetch_gtest()

    add_executable(TestVelyraCore ${VELYRA_CORE_HEADERS} ${VELYRA_CORE_SRC} ${VELYRA_CORE_TEST_SRC})
    target_include_directories(TestVelyraCore PUBLIC include)
    target_precompile_headers(TestVelyraCore PUBLIC test/TestPch.hpp)
    target_link_libraries(TestVelyraCore PUBLIC ${VELYRA_CORE_LIBS} GTest::gtest GTest::gtest_main VelyraUtils)
    target_include_directories(TestVelyraCore PUBLIC include)
    add_test(NAME CTestVelyraCore COMMAND ${PROOT} $<TARGET_FILE:TestVelyraCore>)
    set_tests_properties(CTestVelyraCore PROPERTIES
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif ()

