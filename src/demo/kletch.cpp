#include "prefix.h"

#include <vector>
#include <sstream>

#include <lib/kletch.h>
namespace kletch {
    extern void init_resources();
}
using namespace kletch;

#include "aimer_demo.h"
#include "demo.h"
#include "gl_context_snapshot.h"
#include "hello_demo.h"

gl::ContextSnapshot demo_snapshot;
gl::ContextSnapshot twbar_snapshot;
SDL_Window* window;
SDL_GLContext gl_context;
TwBar* twbar;

std::vector<Demo*> demos;
Demo* demo;
int demo_index = -1;

int init(int argc, char** argv);
int main_loop();
void quit();

int main(int argc, char** argv)
{
    int init_result = init(argc, argv);
    if (init_result != 0)
        return init_result;

    int main_loop_result = main_loop();
    if (main_loop_result != 0)
        return main_loop_result;

    quit();
    return main_loop_result;
}

int init_sdl();
void quit_sdl();

int init_twbar();
void quit_twbar();
//int sdl_keycode_to_twbar(SDL_Keycode code);
//int sdl_keymod_to_twbar(SDL_Keymod mod);

int init_demos();
void quit_demos();

int init(int argc, char** argv)
{
    cout << "Initializing ...\n" << endl;

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

    init_resources();
    cout << "Resource initialization complete" << endl;

    cout << "\nIitialization complete\n" << endl;

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

    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    window = SDL_CreateWindow(
        "Kletch",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (window == nullptr)
    {
        cerr << "Unable to create window: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr)
    {
        cerr << "Unable to create OpenGL context: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window); window = nullptr;
        SDL_Quit();
        return 1;
    }

    int ext_result = gl::init_ext();
    if (ext_result != 0)
    {
        cerr << "Couldn't initialize OpenGL extensions" << endl;
        SDL_GL_DeleteContext(gl_context); gl_context = nullptr;
        SDL_DestroyWindow(window); window = nullptr;
        SDL_Quit();
        return ext_result;
    }

    cout << "SDL initialization complete" << endl;
    cout << "OpenGL Platform:"
        << "\n    OpenGL Version: " << (const char*)glGetString(GL_VERSION)
        << "\n    GLSL Version:   " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)
        << "\n    Vendor:         " << (const char*)glGetString(GL_VENDOR)
        << "\n    Renderer:       " << (const char*)glGetString(GL_RENDERER)
        << endl;

    demo_snapshot.capture();
    twbar_snapshot.capture();

    return 0;
}

void quit_sdl()
{
    assert(window != nullptr);
    assert(gl_context != nullptr);

    SDL_Quit();
    window = nullptr;
}

int init_twbar()
{
    assert(window != nullptr);

    if (TwInit(TW_OPENGL, NULL) == 0)
    {
        cerr << "Unable to initialize AntTweakBar: " << TwGetLastError() << endl;
        return 1;
    }

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    TwWindowSize(w, h);
    twbar = TwNewBar("Kletch");
    float refresh_rate = 1.0f / 30.0f;
    TwSetParam(twbar, nullptr, "refresh", TW_PARAM_FLOAT, 1, &refresh_rate);

    cout << "AntTweakBar initialization complete" << endl;
    return 0;
}

void quit_twbar()
{
    assert(twbar != nullptr);

    twbar_snapshot.restore();
    TwTerminate();
    twbar = nullptr;
}

int init_demos()
{
    demos.emplace_back(new HelloDemo);
    demos.emplace_back(new AimerDemo);
    // Add more demos here

    // Create AntTweakBar combo box
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
    if (demo != nullptr)
    {
        demo_snapshot.restore();
        demo->close();
    }
    for (auto& demo : demos)
        delete demo;
    demos.clear();
}

void draw()
{
    if (demo)
    {
        demo_snapshot.restore();
        demo->render();
        demo_snapshot.capture();
    }
    else
    {
        glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    twbar_snapshot.restore();
    TwDraw();
    twbar_snapshot.capture();

    SDL_GL_SwapWindow(window);
}

int main_loop()
{
    assert(window != nullptr);
    assert(twbar != nullptr);

    SDL_Event e;
    bool running = true;
    int twbar_wheelpos = 0;
    while (running && SDL_WaitEvent(&e))
    {
        bool handled = false;
        bool redraw = false;
        int initial_demo_index = demo_index;

        switch (e.type) {
        case SDL_KEYDOWN:
        {
            auto key = e.key.keysym.sym;
            if (TwKeyPressed(key, e.key.keysym.mod))
            {
                handled = redraw = true;
            }
            else if (key == SDLK_ESCAPE)
            {
                running = false;
            }
            else if (SDLK_1 <= key && key <= SDLK_9 && key - SDLK_1 < demos.size())
            {
                demo_index = key - SDLK_1;
                redraw = true;
            }
            else if (key == SDLK_h)
            {
                int value = 1;
                TwSetParam(twbar, nullptr, "iconified", TW_PARAM_INT32, 1, &value);
                redraw = true;
            }
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            handled = redraw = TwMouseButton(TW_MOUSE_PRESSED, (TwMouseButtonID)e.button.button);
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            handled = redraw = TwMouseButton(TW_MOUSE_RELEASED, (TwMouseButtonID)e.button.button);
            break;
        }
        case SDL_MOUSEMOTION:
        {
            handled = redraw = TwMouseMotion(e.motion.x, e.motion.y);
            break;
        }
        case SDL_MOUSEWHEEL:
        {
            twbar_wheelpos += e.wheel.y;
            handled = redraw = TwMouseWheel(twbar_wheelpos);
            break;
        }
        case SDL_WINDOWEVENT:
        {
            if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                TwWindowSize(e.window.data1, e.window.data2);
                if (demo) demo->resize();
                glViewport(0, 0, e.window.data1, e.window.data2);
                handled = true;
                redraw = true;
            }
            break;
        }
        case SDL_QUIT:
        {
            running = false;
            break;
        }}

        if (!handled && demo)
        {
            handled = demo->on_event(e);
        }
        else if (initial_demo_index != demo_index)
        {
            demo_snapshot.restore();
            if (demo)
            {
                demo->close();
                demo = nullptr;
            }
            if (demo_index >= 0 && demo_index < demos.size())
            {
                demo = demos[demo_index];
                demo->open(window, twbar);
                demo_snapshot.capture();
            }
        }

        if (redraw || (demo && demo->needs_redraw()))
            draw();
    }

    return 0;
}
/*
int sdl_keycode_to_twbar(SDL_Keycode code)
{
    if (' ' <= code && code <= '~') // Printable ASCII character
        returun code;
    switch (code)
    {
        case SDLK_BACKSPACE: return TW_KEY_BACKSPACE;
        case SDLK_TAB: return TW_KEY_TAB;
        case SDLK_CLEAR: return TW_KEY_CLEAR;
        case SDLK_RETURN: return TW_KEY_RETURN;
        case SDLK_PAUSE: return TW_KEY_PAUSE;
        case SDLK_ESCAPE: return TW_KEY_ESCAPE;
        case SDLK_SPACE: return TW_KEY_SPACE;
        case SDLK_DELETE: return TW_KEY_DELETE;
        case SDLK_UP: return TW_KEY_UP;
        case SDLK_DOWN: return TW_KEY_DOWN;
        case SDLK_LEFT: return TW_KEY_LEFT;
        case SDLK_RIGHT: return TW_KEY_RIGHT;
        case SDLK_INSERT: return TW_KEY_INSERT;
        case SDLK_HOME: return TW_KEY_HOME;
        case SDLK_END: return TW_KEY_END;
        case SDLK_PAGEUP: return TW_KEY_PAGE_UP;
        case SDLK_PAGEDOWN: return TW_KEY_PAGE_DOWN;
    }
    if (SDLK_F1 <= code && code <= SDLK_F15)
        return TW_KEY_F1 + (code - SDLK_F1);
}

int sdl_keymod_to_twbar(SDL_Keymod mod)
{
    int result = TW_KMOD_NONE;
    if (mod & KMOD_SHIFT)
        result |= TW_KMOD_SHIFT;
    if (mode & KMOD_CTRL))
        result |= TW_KMOD_CTRL;
    if (mode & KMOD_ALT)
        retult |= TW_KMOD_ALT;
    return result;
}
*/
