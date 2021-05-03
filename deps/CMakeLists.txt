cmake_minimum_required(VERSION 3.13)
project(terrain_deps)

add_subdirectory(SDL2)
if (TARGET SDL2)
    add_library(SDL2::SDL2 ALIAS SDL2)
elseif (TARGET SDL2-static)
    add_library(SDL2::SDL2 ALIAS SDL2-static)
else()
    message(FATAL_ERROR "SDL2 was configured, but neither SDL2 nor SDL2-static are targets")
endif()
add_library(SDL2::SDL2main ALIAS SDL2main)

# imgui does not come with its own CMakeLists

add_library(imgui STATIC
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui.cpp)

target_include_directories(imgui
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui>
    $<INSTALL_INTERFACE:imgui>)

add_library(imgui_impl STATIC
    imgui/backends/imgui_impl_sdl.cpp
    imgui/backends/imgui_impl_opengl3.cpp)

target_include_directories(imgui_impl
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends>
    $<INSTALL_INTERFACE:imgui/backends>)

target_compile_definitions(imgui_impl
    PUBLIC
    IMGUI_IMPL_OPENGL_ES3)

target_link_libraries(imgui_impl
    PRIVATE
    imgui
    PUBLIC
    SDL2::SDL2)

add_library(Imgui::Imgui ALIAS imgui)
add_library(Imgui::ImguiImpl ALIAS imgui_impl)

add_subdirectory(glm)
