#pragma once

#include "SdlWindow.hpp"
#include <SDL_video.h>

namespace Sdl
{
struct GlContext
{
    // TODO: figure out how to make this work with unique_ptr
    // std::unique_ptr<SDL_GLContext, decltype(&SDL_GL_DeleteContext)> context;
    SDL_GLContext ctx;
    GlContext(Window& w, uint32_t flags = 0)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, flags);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        ctx = SDL_GL_CreateContext(w.window.get());
    }

    ~GlContext()
    {
        SDL_GL_DeleteContext(ctx);
    }

};

}