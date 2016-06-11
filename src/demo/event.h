#ifndef KLETCH_DEMO_EVENT_H
#define KLETCH_DEMO_EVENT_H

#include "prefix.h"

namespace kletch {

enum EventType
{
    KEY_PRESS, KEY_RELEASE,
    MOUSE_PRESS, MOUSE_RELEASE, MOUSE_MOVE, MOUSE_SCROLL,
    RESIZE
};

struct Event
{
    EventType type;
    union { int key; int button; };
    union { vec2i pos; vec2i size; };
    int delta;
    int mod;

    Event(EventType type) : type(type), key(-1), pos(0, 0), delta(0), mod(0) { }

    static int translate_glfw_key(int key);
    static int translate_glfw_mods(int mods);
    static int translate_glfw_mouse(int mouse);
};

// Since we have to translate from GLFW to AntTweakBar anyway and GLFW constants are pretty
// cumbersome, lets just define our own that are compatible with AntTweakBar.

constexpr int KEY_BACKSPACE = TW_KEY_BACKSPACE;
constexpr int KEY_TAB = TW_KEY_TAB;
constexpr int KEY_CLEAR = TW_KEY_CLEAR;
constexpr int KEY_RETURN = TW_KEY_RETURN;
constexpr int KEY_PAUSE = TW_KEY_PAUSE;
constexpr int KEY_ESCAPE = TW_KEY_ESCAPE;
constexpr int KEY_DELETE = TW_KEY_DELETE;
constexpr int KEY_UP = TW_KEY_UP;
constexpr int KEY_DOWN = TW_KEY_DOWN;
constexpr int KEY_LEFT = TW_KEY_LEFT;
constexpr int KEY_RIGHT = TW_KEY_RIGHT;
constexpr int KEY_INSERT = TW_KEY_INSERT;
constexpr int KEY_HOME = TW_KEY_HOME;
constexpr int KEY_END = TW_KEY_END;
constexpr int KEY_PAGE_UP = TW_KEY_PAGE_UP;
constexpr int KEY_PAGE_DOWN = TW_KEY_PAGE_DOWN;

constexpr int MOD_SHIFT = TW_KMOD_SHIFT;
constexpr int MOD_CTRL = TW_KMOD_CTRL;
constexpr int MOD_ALT = TW_KMOD_ALT;

constexpr int MOUSE_LEFT = TW_MOUSE_LEFT;
constexpr int MOUSE_RIGHT = TW_MOUSE_RIGHT;
constexpr int MOUSE_MIDDLE = TW_MOUSE_MIDDLE;

} // namespace kletch

#endif // KLETCH_DEMO_EVENT_H
