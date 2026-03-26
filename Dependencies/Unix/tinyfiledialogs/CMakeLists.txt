cmake_minimum_required(VERSION 3.22)

# Create static library for tinyfiledialogs
add_library(tinyfiledialogs STATIC)

# Add source files
target_sources(tinyfiledialogs PRIVATE
    tinyfiledialogs.c
)

# Set include directories (public so consumers can find the header)
target_include_directories(tinyfiledialogs PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Ensure the linker uses C
set_target_properties(tinyfiledialogs PROPERTIES
    LINKER_LANGUAGE C
)
