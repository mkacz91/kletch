#include "prefix.h"
#include "SDL.h"

#include "hello.h"
using namespace kletch;

int main(int argc, char** argv)
{
    Hello hello;
    cout << hello.hello() << endl;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cerr << "Unable to initialize SDL: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Surface* window = SDL_SetVideoMode(800, 600, 0, SDL_SWSURFACE);

    if (window == nullptr)
    {
        cerr << "Unable to create window: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    SDL_Event e;
    bool running = true;
    while (running && SDL_WaitEvent(&e))
    {
        switch (e.type) {
        case SDL_KEYDOWN:
            if (e.key.keysym.sym == SDLK_ESCAPE)
                running = false;
            break;

        case SDL_QUIT:
            running = false;
            break;
        }
    }

    SDL_Quit();
    return 0;
}
