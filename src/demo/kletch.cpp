#include "kletch_prefix.h"

#include "hello.h"
using namespace kletch;

void draw();

int main(int argc, char** argv)
{
    Hello hello;
    cout << hello.hello() << endl;

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

    TwInit(TW_OPENGL, NULL);
    TwWindowSize(window->w, window->h);
    TwBar* twbar = TwNewBar("Hello AntTweakBar!");
    bool b = true;
    TwAddVarRW(twbar, "b", TW_TYPE_BOOLCPP, &b, "");

    SDL_Event e;
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

    SDL_Quit();
    return 0;
}

void draw()
{
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    TwDraw();
    SDL_GL_SwapBuffers();
}
