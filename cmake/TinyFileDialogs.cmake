include(FetchContent)

get_filename_component(VELYRA_CORE_DIR "${CMAKE_CURRENT_LIST_DIR}" DIRECTORY)
set(TFD_DIR "${CMAKE_BINARY_DIR}/_deps/tinyfiledialogs-src")
set(VELYRA_CORE_RFD_DIR "${VELYRA_CORE_DIR}/src/VelyraCore/TinyFileDialogs")

if (NOT EXISTS "${TFD_DIR}/README.md")
    message(STATUS "Fetching TinyFileDialogs library")
    FetchContent_Declare(
        tinyfiledialogs
        GIT_REPOSITORY git@github.com:native-toolkit/libtinyfiledialogs.git
        GIT_TAG master
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(tinyfiledialogs)

    file(GLOB TFD_HPP "${TFD_DIR}/*.h")
    file(GLOB TFD_CPP "${TFD_DIR}/*.c")

    # Copy all found files
    file(COPY ${TFD_HPP} DESTINATION "${VELYRA_CORE_RFD_DIR}")
    file(COPY ${TFD_CPP} DESTINATION "${VELYRA_CORE_RFD_DIR}")
    message(STATUS "tinyfiledialogs library fetched and files copied.")
endif ()

file(GLOB TFD_SOURCES
    CONFIGURE_DEPENDS
    "${VELYRA_CORE_RFD_DIR}/*.c")
add_library(tinyfiledialogs STATIC)
target_sources(tinyfiledialogs PRIVATE ${TFD_SOURCES})
target_include_directories(tinyfiledialogs PUBLIC ${VELYRA_CORE_RFD_DIR})
set_target_properties(tinyfiledialogs PROPERTIES LINKER_LANGUAGE C)