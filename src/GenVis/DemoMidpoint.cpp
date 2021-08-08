#include "MidpointLine.hpp"


#include "SdlObjects/SdlWindow.hpp"
#include "SdlObjects/SdlGlContext.hpp"

#include "Util/Random.hpp"

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <SDL.h>

#include <GLES3/gl3.h>

#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

void tick_trampoline(void* classptr)
{
    Application *app = static_cast<Application*>(classptr);
    app->tick();
}

#endif

constexpr int NUM_GENERATIONS = 12;

// Global state should be saved here
struct Application
{
    std::unique_ptr<Sdl::Window> window;
    std::unique_ptr<Sdl::GlContext> context;

    rng::Xorshift128p generator;
    rng::Xorshift128p::state savedGeneratorState;

    int currentGeneration = 0;

    std::vector<MidpointLine> lines;

    bool should_quit;

    float roughness = 0.2;

    Application(int argc, char** argv);
    ~Application();

    void tick();

    void rebuildLines();

    void drawMidpointLine();

    int run();

};

Application::Application(int argc, char** argv)
{
    (void) argc;
    (void) argv;
    generator.currState.a = 10123210;
    generator.currState.b = 23210412;
    savedGeneratorState = generator.currState;
    SDL_Init(SDL_INIT_EVERYTHING);
}

Application::~Application()
{
    context.reset();
    window.reset();
    SDL_Quit();
}

int Application::run()
{
    window.reset(new Sdl::Window("terrain", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE));
    context.reset(new Sdl::GlContext(*window));

    rebuildLines();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window->window.get(), context->ctx);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    io.Fonts->AddFontDefault();

    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(tick_trampoline, (void*)this, 0, true);
    #else
    while(!should_quit)
    {
        tick();
    }
    #endif

    return 0;
}

void Application::rebuildLines()
{
    lines.resize(NUM_GENERATIONS);
    generator.currState = savedGeneratorState;
    lines[0] = MidpointLine(generator, roughness);
    for(size_t generation = 1; generation < NUM_GENERATIONS; ++generation)
    {
        lines[generation] = lines[generation - 1].nextGeneration(generator);
    }
}

void Application::drawMidpointLine()
{
    ImGuiIO &io = ImGui::GetIO();
    ImGui::SetNextWindowSizeConstraints(ImVec2(500, 200), io.DisplaySize);
    ImGui::Begin("Line preview");

    auto drawList = ImGui::GetWindowDrawList();
    auto win = ImGui::GetWindowContentRegionMin();
    auto width = ImGui::GetWindowWidth();
    auto height = ImGui::GetWindowHeight();
    auto offset = ImGui::GetWindowPos();

    for(size_t gen = 0; gen <= currentGeneration; ++gen)
    {
        for(size_t i = 0; i < lines[gen].points.size() - 1; ++i)
        {
            auto &p1 = lines[gen].points[i];
            auto &p2 = lines[gen].points[i + 1];
            auto x1 = p1.x * width + offset.x;
            auto y1 = - (p1.y * height) + height / 2.0f + offset.y;
            auto x2 = p2.x * width + offset.x;
            auto y2 = - (p2.y * height) + height / 2.0f + offset.y;
            float alpha = (gen + 1.0f) / (currentGeneration + 1.0f);
            drawList->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImColor(1.0f, 1.0f, 0.0f, alpha));
        }
    }

    ImGui::End();
}

void Application::tick()
{
    ImGuiIO io = ImGui::GetIO();

    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (SDL_QUIT == event.type)
        {
            should_quit = true;
        }
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window->window.get());

    ImGui::NewFrame();

    ImGui::SetNextWindowSizeConstraints(ImVec2(300, 150), io.DisplaySize);
    ImGui::Begin("Midpoint parameters");

    if (ImGui::SliderFloat("Roughness", &roughness, 0, 1))
    {
        std::cout << "Roughness changed to " << roughness << std::endl;
        rebuildLines();
    }

    if (ImGui::SliderInt("Generation", &currentGeneration, 0, NUM_GENERATIONS - 1))
    {
        std::cout << "Requested generation changed to " << currentGeneration << std::endl;
    }

    drawMidpointLine();

    ImGui::End();

    ImGui::Render();

    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window->window.get());
}


int main(int argc, char** argv)
{
    Application app(argc, argv);
    return app.run();
}
