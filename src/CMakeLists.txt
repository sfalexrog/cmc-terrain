cmake_minimum_required(VERSION 3.13)
project(terrain_main)

add_executable(terrain
    Application.cpp
    main.cpp)

target_link_libraries(terrain PUBLIC
    GLESv2
    SDL2::SDL2
    SDL2::SDL2main
    Imgui::Imgui
    Imgui::ImguiImpl)

if(EMSCRIPTEN)
    set_target_properties(terrain
        PROPERTIES
            SUFFIX .html)
    target_link_options(terrain
        PRIVATE
            --shell-file ${EMSCRIPTEN_BASE_HTML}
            -s MAX_WEBGL_VERSION=2)
endif()
