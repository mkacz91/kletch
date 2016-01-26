#include "prefix.h"

#include <lib/hello.h>
using namespace kletch;

SDL_Surface* window;
TwBar* twbar;
int demo_index = -1;
std::vector<unique_ptr<Demo>> demos;

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


    bool b = true;
    TwAddVarRW(twbar, "b", TW_TYPE_BOOLCPP, &b, "");

    SDL_Quit();
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

    Uint32 video_flags = SDL_OPENGL | SDL_RESIZABLE;
    SDL_Surface* window = SDL_SetVideoMode(800, 600, 0, video_flags);
    if (window == nullptr)
    {
        cerr << "Unable to create window: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

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
    assert(window != nullptr);

    TwInit(TW_OPENGL, NULL);
    TwWindowSize(window->w, window->h);
    TwBar* twbar = TwNewBar("Kletch");

    
}

void quit_twbar()
{
    assert(twbar != nullptr);

    TwTerminate();
    twbar = nullptr;
}

void quit_twbar()
{
    TwTerminate();
}

void draw()
{
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    TwDraw();
    SDL_GL_SwapBuffers();
}

void init_demos()
{
    demos.emplace_back(new HelloDemo);
    // Add more demos here
}

void quit_demos()
{
    if (demo_index >= 0 && demo_index < demos.size())
        demos[demo_index]->clean_up(true);
}


void main_loop(SDL_Surface* window, TwBar* twbar)
{
    assert(window != nullptr);
    assert(twbar != nullptr);

    DemoEvent e;
    bool running = true;
    bool redraw = true;
    while (running && SDL_WaitEvent(&e))
    {
        if (TwEventSDL(&e, SDL_MAJOR_VERSION, SDL_MINOR_VERSION))
        {
            redraw = true;
        }
        else
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

        if (running && redraw)
            draw();
        redraw = false;
    }
}
