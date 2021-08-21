#include "SdlGlContext.hpp"

// FIXME: make sure the proper SDL_config.h is used here
#include <SDL_config.h>
#include <SDL_syswm.h>

namespace
{

EGLNativeWindowType getNativeWindow(SDL_Window *w)
{
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL_GetWindowWMInfo(w, &info))
    {
        switch(info.subsystem)
        {
            case SDL_SYSWM_UNKNOWN:
                return NULL;
#if defined(SDL_VIDEO_DRIVER_WINDOWS)
            case SDL_SYSWM_WINDOWS:
                return (EGLNativeWindowType)info.info.window;
#endif
#if defined(SDL_VIDEO_DRIVER_WINRT)
            case SDL_SYSWM_WINRT:
                return (EGLNativeWindowType)info.info.winrt.window;
#endif
#if defined(SDL_VIDEO_DRIVER_X11)
            case SDL_SYSWM_X11:
                return (EGLNativeWindowType)info.info.x11.window;
#endif
#if defined(SDL_VIDEO_DRIVER_DIRECTFB)
            case SDL_SYSWM_DIRECTFB:
                return (EGLNativeWindowType)info.info.dfb.window;
#endif
#if defined(SDL_VIDEO_DRIVER_COCOA)
            case SDL_SYSWM_COCOA:
                return (EGLNativeWindowType)info.info.cocoa.window;
#endif
#if defined(SDL_VIDEO_DRIVER_UIKIT)
            case SDL_SYSWM_UIKIT:
                return (EGLNativeWindowType)info.info.uikit.window;
#endif
#if defined(SDL_VIDEO_DRIVER_WAYLAND)
            case SDL_SYSWM_WAYLAND:
                return (EGLNativeWindowType)info.info.wl.egl_window;
#endif
#if defined(SDL_VIDEO_DRIVER_ANDROID)
            case SDL_SYSWM_ANDROID:
                return (EGLNativeWindowType)info.info.android.window;
#endif
#if defined(SDL_VIDEO_DRIVER_OS2)
            case SDL_SYSWM_OS2:
                return (EGLNativeWindowType)info.info.os2.window;
#endif
#if defined(SDL_VIDEO_DRIVER_VIVANTE)
            case SDL_SYSWM_VIVANTE:
                return (EGLNativeWindowType)info.info.vivante.window;
#endif
#if defined(SDL_VIDEO_DRIVER_KMSDRM)
            case SDL_SYSWM_KMSDRM:
                // FIXME: This might be wrong
                return (EGLNativeWindowType)info.info.kmsdrm.gbm_dev;
#endif
            default:
                std::cerr << "Unknown subsystem: " << info.subsystem << std::endl;
                return NULL;
        }
    }
    else
    std::cerr << "Failed to obtain native window info" << std::endl;
    return NULL;
}

} // anonymous namespace

namespace Sdl
{

GlContext::GlContext(Window& w, uint32_t flags): dpy(EGL_NO_DISPLAY), major(0), minor(0)
{
    // 1. Initialize display connection
    dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    auto initResult = eglInitialize(dpy, &major, &minor);
    if (!initResult)
    {
        throw std::logic_error("Failed to initialize EGL display connection");
    }
    // 1.5. Log display connection details
    std::cout << "EGL vendor: " << eglQueryString(dpy, EGL_VENDOR) << std::endl;
    std::cout << "EGL version: " << eglQueryString(dpy, EGL_VERSION) << std::endl;
    std::cout << "EGL extensions: " << eglQueryString(dpy, EGL_EXTENSIONS) << std::endl;
    std::cout << "EGL client APIs: " << eglQueryString(dpy, EGL_CLIENT_APIS) << std::endl;

    // 2. Get a suitable config
    const EGLint attribList[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_NONE, EGL_NONE
    };

    EGLConfig configs[50];
    EGLint numConfigs = 0;
    eglChooseConfig(dpy, attribList, configs, 50, &numConfigs);
    std::cout << "Number of suitable configs: " << numConfigs;
    if (numConfigs == 0)
    {
        throw std::logic_error("Failed to obtain a suitable config");
    }

    // 3. Create a native window surface

    EGLNativeWindowType sysWindow = getNativeWindow(w.window.get());
    // Note that for some WMs, a NULL window is still fine
    windowSurface = eglCreateWindowSurface(dpy, configs[0], sysWindow, nullptr);

    // 4. Create EGL context
    eglBindAPI(EGL_OPENGL_ES_API);
    const EGLint contextAttribs[] = {
        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_CONTEXT_MINOR_VERSION, 0,
        EGL_CONTEXT_OPENGL_DEBUG, (flags & SDL_GL_CONTEXT_DEBUG_FLAG) ? EGL_TRUE : EGL_FALSE,
        EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE, (flags & SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG) ? EGL_TRUE : EGL_FALSE,
        EGL_CONTEXT_OPENGL_ROBUST_ACCESS, (flags & SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG) ? EGL_TRUE : EGL_FALSE,
        EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY, (flags | SDL_GL_CONTEXT_RESET_ISOLATION_FLAG) ? EGL_LOSE_CONTEXT_ON_RESET : EGL_NO_RESET_NOTIFICATION,
        EGL_NONE, EGL_NONE
    };
    ctx = eglCreateContext(dpy, configs[0], EGL_NO_CONTEXT, contextAttribs);

    // 5. Connect context to surface
    eglMakeCurrent(dpy, windowSurface, windowSurface, ctx);

/*
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, flags);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    ctx = SDL_GL_CreateContext(w.window.get()); */
}

bool GlContext::swap()
{
    return eglSwapBuffers(dpy, windowSurface);
}

GlContext::~GlContext()
{
    eglMakeCurrent(dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(dpy, ctx);
    eglDestroySurface(dpy, windowSurface);
    eglTerminate(dpy);
}

}
