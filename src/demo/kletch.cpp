#include "prefix.h"
#include "printing.h"

#include <lib/kletch.h>

#include "aimer_demo.h"
#include "demo.h"
#include "fitter_demo.h"
#include "constrained_fitter_demo.h"
#include "gl_context_snapshot.h"
#include "hello_demo.h"

namespace kletch
{
    ShaderAssetPack Assets::shaders;
}
using namespace kletch;

int init(int argc, char** argv);
void main_loop();
int quit();

int main(int argc, char** argv)
{
    try
    {
        int init_result = init(argc, argv);
        if (init_result != 0)
            return init_result;
        main_loop();
        return quit();
    }
    catch (std::exception& e)
    {
        cerr << "\n--- FATAL ERROR ---\n\n" << e.what() << endl;
        return 1;
    }
}

int init_assets();
int quit_assets();
int init_glfw();
int quit_glfw();
void glfw_error_callback(int error, char const* description);
void on_key(GLFWwindow* window, int key, int scancode, int action, int mods);
void on_mouse_button(GLFWwindow* window, int button, int action, int mods);
void on_mouse_move(GLFWwindow* window, double x, double y);
void on_mouse_scroll(GLFWwindow* window, double x, double y);
void on_resize(GLFWwindow* window, int width, int height);
void draw();
bool redraw = true;
GLFWwindow* window;
int glfw_last_error = 1;
char const* glfw_last_error_description = "";
vec2i mouse_pos = 0;
int keyboard_mod = 0;
enum MouseFocus { MOUSE_FOCUS_NONE, MOUSE_FOCUS_DEMO, MOUSE_FOCUS_TWBAR };
MouseFocus mouse_focus = MOUSE_FOCUS_NONE;

int init_twbar();
int quit_twbar();
TwBar* twbar;
gl::ContextSnapshot twbar_snapshot;
int twbar_wheelpos = 0;

int init_demos();
int quit_demos();
std::vector<Demo*> demos;
Demo* demo = nullptr;
int demo_index = 0;
gl::ContextSnapshot demo_snapshot;

typedef int (*init_fun_t)();
typedef int (*quit_fun_t)();
typedef tuple<init_fun_t, quit_fun_t> subsystem_t;
subsystem_t subsystems[] =
{
    make_tuple(init_assets, quit_assets),
    make_tuple(init_glfw, quit_glfw),
    make_tuple(init_twbar, quit_twbar),
    make_tuple(init_demos, quit_demos)
};
constexpr int SUBSYSTEM_COUNT = sizeof(subsystems) / sizeof(subsystem_t);
int init_subsystem_count = 0;

int init(int argc, char** argv)
{
    cout << "\nInitializing ...\n" << endl;
    assert(init_subsystem_count == 0);
    for (;init_subsystem_count < SUBSYSTEM_COUNT; ++init_subsystem_count)
    {
        init_fun_t init_fun = get<0>(subsystems[init_subsystem_count]);
        int result = init_fun();
        if (result != 0)
        {
            quit(); // Ignore the return value
            return result;
        }
    }
    cout << "\nIitialization complete\n" << endl;

    return 0;
}

int quit()
{
    cout << "\nShutting down ...\n" << endl;
    int result = 0;
    while (init_subsystem_count --> 0)
    {
        quit_fun_t quit_fun = get<1>(subsystems[init_subsystem_count]);
        int sub_result = quit_fun();
        // Keep the first error
        if (result == 0 && sub_result != 0)
            result = sub_result;
    }
    return result;
}

int init_assets()
{
    cout << "Initializing Assets ... " << flush;
    Assets::shaders.set_root_dir("assets/shaders");
    cout << "done" << endl;
    return 0;
}

int quit_assets() { return 0; }

int init_glfw()
{
    cout << "Initializing GLFW ... " << flush;
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        cerr << "Unable to initialize GLFW: " << glfw_last_error << " "
            << glfw_last_error_description << endl;
        return glfw_last_error;
    }

    window = glfwCreateWindow(800, 600, "Kletch", nullptr, nullptr);
    if (!window)
    {
        cerr << "Unable to create window: " << glfw_last_error << " "
            << glfw_last_error_description << endl;
        glfwTerminate();
        return glfw_last_error;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cerr << "Unable to load OpenGL extensions" << endl;
        glfwTerminate();
        window = nullptr;
        return 1;
    }

    glfwSwapInterval(1);
    glfwSetKeyCallback(window, on_key);
    glfwSetMouseButtonCallback(window, on_mouse_button);
    glfwSetCursorPosCallback(window, on_mouse_move);
    glfwSetScrollCallback(window, on_mouse_scroll);
    glfwSetFramebufferSizeCallback(window, on_resize);

    cout << "done" << endl;
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

int quit_glfw()
{
    cout << "Closing GLFW ... " << flush;
    assert(window != nullptr);
    glfw_last_error = 0;
    glfwDestroyWindow(window);
    window = nullptr;
    glfwTerminate(); // No errors
    if (glfw_last_error != 0)
        cerr << glfw_last_error_description << endl;
    else
        cout << "done" << endl;
    return glfw_last_error;
}

void glfw_error_callback(int error, char const* description)
{
    glfw_last_error = error;
    glfw_last_error_description = description;
}

void on_key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ignore(window, scancode);
    Event e(action == GLFW_PRESS || action == GLFW_REPEAT ? KEY_PRESS : KEY_RELEASE);
    e.key = Event::translate_glfw_key(key);
    e.mod = keyboard_mod = Event::translate_glfw_mods(mods);
    e.pos = mouse_pos;
    if (e.type == KEY_PRESS)
    {
        if (TwKeyPressed(e.key, e.mod))
        {
            redraw = true;
            return;
        }
        if (key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window, 1);
            return;
        }
        if (GLFW_KEY_0 <= key && key <= GLFW_KEY_9)
        {
            demo_index = key - GLFW_KEY_0;
            return;
        }
    }
    if (demo)
        demo->on_event(e);
}

void on_mouse_button(GLFWwindow* window, int button, int action, int mods)
{
    ignore(window, mods);
    Event e(action == GLFW_PRESS ? MOUSE_PRESS : MOUSE_RELEASE);
    e.button = Event::translate_glfw_mouse(button);
    e.mod = keyboard_mod = Event::translate_glfw_mods(mods);
    e.pos = mouse_pos;
    TwMouseAction tw_action = action == GLFW_PRESS ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;
    if (mouse_focus != MOUSE_FOCUS_DEMO && TwMouseButton(tw_action, (TwMouseButtonID)e.button))
    {
        mouse_focus = MOUSE_FOCUS_TWBAR;
        redraw = true;
    }
    else if (mouse_focus != MOUSE_FOCUS_TWBAR && demo && demo->on_event(e))
    {
        mouse_focus = MOUSE_FOCUS_DEMO;
    }
    if (e.type == MOUSE_RELEASE)
        mouse_focus = MOUSE_FOCUS_NONE;
}

void on_mouse_move(GLFWwindow* window, double x, double y)
{
    ignore(window);
    Event e(MOUSE_MOVE);
    e.pos = mouse_pos = vec2i(x, y);
    e.mod = keyboard_mod;
    if (mouse_focus != MOUSE_FOCUS_DEMO && TwMouseMotion(x, y))
        redraw = true;
    else if (mouse_focus != MOUSE_FOCUS_TWBAR && demo)
        demo->on_event(e);
}

void on_mouse_scroll(GLFWwindow* window, double x, double y)
{
    ignore(window, x);
    Event e(MOUSE_SCROLL);
    e.delta = y > 0 ? 1 : -1;
    e.pos = mouse_pos;
    e.mod = keyboard_mod;
    twbar_wheelpos += e.delta;
    if (TwMouseWheel(twbar_wheelpos))
        redraw = true;
    else if (demo)
        demo->on_event(e);
}

void on_resize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Event e(RESIZE);
    e.size = vec2i(width, height);
    e.mod = keyboard_mod;
    TwWindowSize(width, height);
    if (demo)
        demo->on_event(e);
    draw();
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

    glfwSwapBuffers(window);
    redraw = false;
}

int init_twbar()
{
    cout << "Initializing AntTweakBar ... " << flush;
    assert(window != nullptr);

    if (TwInit(TW_OPENGL, NULL) == 0)
    {
        cerr << "Unable to initialize AntTweakBar: " << TwGetLastError() << endl;
        return 1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    TwWindowSize(width, height);
    twbar = TwNewBar("Kletch");
    float refresh_rate = 1.0f / 30.0f;
    TwSetParam(twbar, nullptr, "refresh", TW_PARAM_FLOAT, 1, &refresh_rate);

    cout << "done" << endl;
    return 0;
}

int quit_twbar()
{
    cout << "Closing AntTweakBar ... " << flush;
    assert(twbar != nullptr);
    twbar_snapshot.restore();
    int result = 1 - TwTerminate();
    twbar = nullptr;
    if (result != 0)
        cerr << TwGetLastError() << endl;
    else
        cout << "done" << endl;
    return result;
}

int init_demos()
{
    cout << "Initializing demos ..." << endl;
    demos.push_back(nullptr);
    demos.push_back(new HelloDemo);
    demos.push_back(new AimerDemo);
    demos.push_back(new FitterDemo);
    demos.push_back(new ConstrainedFitterDemo);
    // Add more demos here

    // Create AntTweakBar combo box
    std::ostringstream enum_string;
    enum_string << "-";
    for (int i = 1; i < demos.size(); ++i)
    {
        Demo const* demo = demos[i];
        cout << "    " << i << ": " << demo->display_name() << endl;
        enum_string << "," << demo->display_name();
    }

    // TwDefineEnumFromString has vaguely documented errors. Leave unchecked and hope for the best.
    TwType demo_twtype = TwDefineEnumFromString("DemoEnum", enum_string.str().c_str());
    int result = 1 - TwAddVarRW(twbar, "Demo", demo_twtype, &demo_index, nullptr);
    if (result != 0)
        cerr << TwGetLastError() << endl;
    else
        cout << "Demos initialization complete" << endl;

    return result;
}

int quit_demos()
{
    cout << "Closing demos ... " << flush;
    if (demo)
    {
        demo_snapshot.restore();
        demo->close();
    }
    for (auto& demo : demos)
        delete demo;
    demos.clear();
    cout << "done" << endl;
    return 0; // TODO: Any errors?
}

void main_loop()
{
    assert(window != nullptr);
    assert(twbar != nullptr);
    while (!glfwWindowShouldClose(window))
    {
        if (redraw || (demo && demo->needs_redraw()))
            draw();
        glfwWaitEvents();

        // Switch demo if necessary
        if (0 <= demo_index && demo_index < demos.size() && demos[demo_index] != demo)
        {
            demo_snapshot.restore();
            if (demo)
                demo->close();
            demo = demos[demo_index];
            if (demo)
            {
                // An artificial resize event is supplied on every open
                Event resize_event(RESIZE);
                glfwGetFramebufferSize(window, &resize_event.size.x, &resize_event.size.y);
                resize_event.mod = keyboard_mod;
                demo->open(window, twbar, resize_event);
                demo_snapshot.capture();
            }
            redraw = true;
        }
    }
}
