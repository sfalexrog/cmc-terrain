#pragma once

//#include "SdlObjects/SdlWindow.hpp"
#include <memory>

namespace Sdl
{
struct Window;
struct GlContext;
}

class Application
{
private:
    std::unique_ptr<Sdl::Window> window;
    std::unique_ptr<Sdl::GlContext> context;
    bool should_quit;

public:
    Application(int argc, char** argv);
    ~Application();

    void tick();

    int run();

};
