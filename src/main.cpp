#include "SdlObjects/SdlWindow.hpp"
#include "Application.hpp"
#include <SDL.h>

int main(int argc, char** argv)
{
    Application app(argc, argv);
    return app.run();
}
