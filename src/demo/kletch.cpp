#include "prefix.h"
#include <vector>
#include <sstream>

#include <lib/kletch.h>
using namespace kletch;

#include "demo.h"
#include "hello_demo.h"

SDL_Surface* window;
Uint32 video_flags;

TwBar* twbar;

std::vector<unique_ptr<Demo>> demos;
int demo_index = -1;

int init(int argc, char** argv);
int main_loop();
void quit();

int main(int argc, char** argv)
{
    int init_result = init(argc, argv);

    int main_loop_result = 2;
    if (init_result == 0)
    {
        try
        {
            main_loop_result = main_loop();
        }
        // If any exceptions reach up here, we don't bother cleaning up
        catch (const std::exception& e)
        {
            cerr << e.what() << endl;
            return 1;
        }
        catch (...)
        {
            cerr << "Unknown exception caught" << endl;
            return 1;
        }
    }
    else
    {
        return init_result;
    }

    quit();
    return main_loop_result;
}

int init_sdl();
void quit_sdl();

int init_twbar();
void quit_twbar();

int init_demos();
void quit_demos();

int init(int argc, char** argv)
{
    int init_sdl_result = init_sdl();
    if (init_sdl_result != 0)
        return init_sdl_result;

    int init_twbar_result = init_twbar();
    if (init_twbar_result != 0)
    {
        quit_sdl();
        return init_twbar_result;
    }

    int init_demos_result = init_demos();
    if (init_demos_result != 0)
    {
        quit_twbar();
        quit_sdl();
        return init_demos_result;
    }

    return 0;
}

void quit()
{
    quit_demos();
    quit_twbar();
    quit_sdl();
}

int init_sdl()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cerr << "Unable to initialize SDL: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    video_flags = SDL_OPENGL | SDL_RESIZABLE;
    window = SDL_SetVideoMode(800, 600, 0, video_flags);
    if (window == nullptr)
    {
        cerr << "Unable to create window: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    cout << "SDL initialization complete" << endl;
    cout << "OpenGL Platform:"
        << "\n    OpenGL Version: " << (const char*)glGetString(GL_VERSION)
        << "\n    GLSL Version:   " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)
        << "\n    Vendor:         " << (const char*)glGetString(GL_VENDOR)
        << "\n    Renderer:       " << (const char*)glGetString(GL_RENDERER)
        << endl;

    return 0;
}

void quit_sdl()
{
    assert(window != nullptr);

    SDL_Quit();
    window = nullptr;
}

int init_twbar()
{
    //assert(window != nullptr);

    if (TwInit(TW_OPENGL, NULL) == 0)
    {
        cerr << "Unable to initialize AntTweakBar: " << TwGetLastError() << endl;
        return 1;
    }

    TwWindowSize(window->w, window->h);
    twbar = TwNewBar("Kletch");

    cout << "AntTweakBar initialization complete" << endl;
    return 0;
}

void quit_twbar()
{
    assert(twbar != nullptr);

    TwTerminate();
    twbar = nullptr;
}

void draw()
{
    if (demo_index >= 0 && demo_index < demos.size())
    {
        demos[demo_index]->render();
    }
    else
    {
        glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    TwDraw();

    SDL_GL_SwapBuffers();
}

int init_demos()
{
    demos.emplace_back(new HelloDemo);
    // Add more demos here

    // Greate AntTweakBar combo box
    std::ostringstream enum_string;
    for (auto& demo : demos)
        enum_string << demo->display_name() << ",";
    TwType demo_twtype = TwDefineEnumFromString("DemoEnum", enum_string.str().c_str());
    TwAddVarRW(twbar, "Demo", demo_twtype, &demo_index, nullptr);

    cout << "Demos initialization complete" << endl;
    return 0;
}

void quit_demos()
{
    if (demo_index >= 0 && demo_index < demos.size())
        demos[demo_index]->close(true);
}

int main_loop()
{
    assert(window != nullptr);
    assert(twbar != nullptr);

    DemoEvent de;
    SDL_Event& e = de.sdl_event();
    bool running = true;
    bool redraw = true;
    while (running && SDL_WaitEvent(&e))
    {
        bool handled = false;
        int initial_demo_index = demo_index;

        // Let AntTweakBar handle the event
        if (running && !handled)
        {
            handled = TwEventSDL(&e, SDL_MAJOR_VERSION, SDL_MINOR_VERSION);
            redraw = redraw || handled;
        }

        // Exchange demo if it was changed
        if (initial_demo_index != demo_index)
        {
            if (initial_demo_index >= 0 && initial_demo_index < demos.size())
                demos[initial_demo_index]->close(true);

            if (demo_index >= 0 && demo_index < demos.size())
                demos[demo_index]->init(window);
        }

        // Handle most basic high priority events
        if (running && !handled)
        {
            switch (e.type) {
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                break;

            case SDL_VIDEORESIZE:
                window = SDL_SetVideoMode(e.resize.w, e.resize.h, 0, video_flags);
                redraw = true;
                break;

            case SDL_QUIT:
                running = false;
                break;
            }
        }

        // Let demo handle the event
        if (running && !handled && demo_index >= 0 && demo_index < demos.size())
        {
            de.reset();
            demos[demo_index]->handle_event(de);
            handled = de.handled();
            redraw = redraw || de.redraw_requested();
            running = !de.quit_requested();
        }

        if (running && redraw)
            draw();
        redraw = false;
    }

    return 0;
}
