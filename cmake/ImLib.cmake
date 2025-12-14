include(FetchContent)

get_filename_component(VELYRA_CORE_DIR "${CMAKE_CURRENT_LIST_DIR}" DIRECTORY)
set(IMGUI_DIR "${CMAKE_BINARY_DIR}/_deps/git_imgui-src")
set(IMPLOT_DIR "${CMAKE_BINARY_DIR}/_deps/git_implot-src")
set(VELYRA_CORE_IMGUI_INCLUDE_DIR "${VELYRA_CORE_DIR}/include/VelyraCore/ImGui")
set(VELYRA_CORE_IMGUI_SRC_DIR "${VELYRA_CORE_DIR}/src/VelyraCore/ImGui")

if (NOT EXISTS "${IMGUI_DIR}/imgui.h" OR NOT EXISTS "${VELYRA_CORE_IMGUI_INCLUDE_DIR}/imgui.h")
    message(STATUS "Fetching ImGui library")
    FetchContent_Declare(
        git_imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG docking
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(git_imgui)

    file(GLOB IMGUI_HPP "${IMGUI_DIR}/*.h")
    file(GLOB IMGUI_CPP "${IMGUI_DIR}/*.cpp")
    file(GLOB IMGUI_BACKENDS_HPP "${IMGUI_DIR}/backends/imgui_impl_*.h" "${IMGUI_DIR}/backends/imgui_impl_*.hpp")
    file(GLOB IMGUI_BACKENDS_CPP "${IMGUI_DIR}/backends/imgui_impl_*.cpp")
    file(GLOB IMGUI_STDLIB_HPP "${IMGUI_DIR}/misc/cpp/*.h")
    file(GLOB IMGUI_STDLIB_CPP "${IMGUI_DIR}/misc/cpp/*.cpp")

    # Copy all found files
    file(COPY ${IMGUI_HPP} DESTINATION "${VELYRA_CORE_IMGUI_INCLUDE_DIR}")
    file(COPY ${IMGUI_CPP} DESTINATION "${VELYRA_CORE_IMGUI_SRC_DIR}")
    file(COPY ${IMGUI_BACKENDS_HPP} DESTINATION "${VELYRA_CORE_IMGUI_SRC_DIR}")
    file(COPY ${IMGUI_BACKENDS_CPP} DESTINATION "${VELYRA_CORE_IMGUI_SRC_DIR}")
    file(COPY ${IMGUI_STDLIB_HPP} DESTINATION "${VELYRA_CORE_IMGUI_INCLUDE_DIR}")
    file(COPY ${IMGUI_STDLIB_CPP} DESTINATION "${VELYRA_CORE_IMGUI_SRC_DIR}")
    message(STATUS "ImGui library fetched and files copied.")
endif ()

if (NOT EXISTS "${IMPLOT_DIR}/implot.h" OR NOT EXISTS "${VELYRA_CORE_IMGUI_INCLUDE_DIR}/implot.h")
    message(STATUS "Fetching ImPlot library")
    FetchContent_Declare(
        git_implot
        GIT_REPOSITORY https://github.com/epezent/implot.git
        GIT_TAG master
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(git_implot)

    file(GLOB IMPLOT_HPP "${IMPLOT_DIR}/*.h")
    file(GLOB IMPLOT_CPP "${IMPLOT_DIR}/*.cpp")
    # Copy all found files
    file(COPY ${IMPLOT_HPP} DESTINATION "${VELYRA_CORE_IMGUI_INCLUDE_DIR}")
    file(COPY ${IMPLOT_CPP} DESTINATION "${VELYRA_CORE_IMGUI_SRC_DIR}")
    message(STATUS "ImPlot library fetched and files copied.")
endif ()

set(IMGUI_PLATFORM_SRC)
if (WIN32)
    set(IMGUI_PLATFORM_SRC
        ${VELYRA_CORE_IMGUI_SRC_DIR}/imgui_impl_win32.cpp
    )
elseif (UNIX)
    message(WARNING "Please add UNIX configuration")
endif ()

set(IMGUI_SRC
    ${IMGUI_PLATFORM_SRC}

    ${VELYRA_CORE_IMGUI_SRC_DIR}/imgui.cpp
    ${VELYRA_CORE_IMGUI_SRC_DIR}/imgui_demo.cpp
    ${VELYRA_CORE_IMGUI_SRC_DIR}/imgui_draw.cpp
    ${VELYRA_CORE_IMGUI_SRC_DIR}/imgui_tables.cpp
    ${VELYRA_CORE_IMGUI_SRC_DIR}/imgui_widgets.cpp

    ${VELYRA_CORE_IMGUI_SRC_DIR}/imgui_impl_opengl3.cpp

    ${VELYRA_CORE_IMGUI_SRC_DIR}/imgui_stdlib.cpp
)

set(IMPLOT_SRC
    ${VELYRA_CORE_IMGUI_SRC_DIR}/implot.cpp
    ${VELYRA_CORE_IMGUI_SRC_DIR}/implot_demo.cpp
    ${VELYRA_CORE_IMGUI_SRC_DIR}/implot_items.cpp
)

add_library(ImLib STATIC ${IMGUI_SRC} ${IMPLOT_SRC})
target_include_directories(ImLib PUBLIC ${VELYRA_CORE_IMGUI_INCLUDE_DIR})