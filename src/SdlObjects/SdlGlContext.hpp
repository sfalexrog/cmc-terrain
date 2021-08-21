#pragma once

#include "SdlWindow.hpp"

#define EGL_NO_X11
#include <EGL/egl.h>
#undef EGL_NO_X11

#include <iostream>

namespace Sdl
{
struct GlContext
{
    // TODO: figure out how to make this work with unique_ptr
    // std::unique_ptr<SDL_GLContext, decltype(&SDL_GL_DeleteContext)> context;
    EGLContext ctx;
    EGLDisplay dpy;
    EGLSurface windowSurface;
    EGLint major;
    EGLint minor;
    GlContext(Window& w, uint32_t flags = 0);
    bool swap();
    ~GlContext();
};

}