#pragma once

#include <SDL_config.h>
#include <SDL_video.h>
#include <memory>

namespace Sdl
{

struct Window
{
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    Window(const char* title,
    int pos_x, int pos_y, int width, int height, uint32_t flags) : window(SDL_CreateWindow(title, pos_x, pos_y, width, height, flags), SDL_DestroyWindow) {}
};

}
