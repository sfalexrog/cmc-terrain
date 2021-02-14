cmake_minimum_required(VERSION 3.13)
project(terrain)

# Use our template for Emscripten
if (EMSCRIPTEN)
    set(EMSCRIPTEN_BASE_HTML ${CMAKE_CURRENT_SOURCE_DIR}/web_tools/base.html)
endif()

add_subdirectory(deps)
add_subdirectory(src)
