cmake_minimum_required(VERSION 3.13)
project(terrain_main)

add_library(terrain_libs SHARED
    SdlObjects/SdlGlContext.cpp
    Util/Random.cpp)

target_include_directories(terrain_libs PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:.>)

target_link_libraries(terrain_libs PUBLIC
    GLESv2
    EGL
    SDL2::SDL2
    SDL2::SDL2main
    Imgui::Imgui
    Imgui::ImguiImpl
    glm::glm)

add_executable(terrain
    Application.cpp
    main.cpp)

target_link_libraries(terrain PUBLIC
    terrain_libs)

add_executable(GenVis
    GenVis/DemoMidpoint.cpp
    GenVis/MidpointLine.cpp)

target_link_libraries(GenVis PUBLIC
    terrain_libs)

if(EMSCRIPTEN)
    set_target_properties(terrain
        PROPERTIES
            SUFFIX .html)
    target_link_options(terrain
        PRIVATE
            --shell-file ${EMSCRIPTEN_BASE_HTML}
            -s MAX_WEBGL_VERSION=2)
endif()
