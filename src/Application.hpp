#pragma once

#include "SdlObjects/SdlWindow.hpp"
#include <memory>

class Application
{
private:
    std::unique_ptr<Sdl::Window> window;
    bool should_quit;
    SDL_GLContext gl_ctx;

public:
    Application(int argc, char** argv);
    ~Application();

    void tick();

    int run();

};
