#include "event.h"

namespace kletch {

int Event::translate_glfw_key(int key)
{
    if (' ' <= key && key <= '~') // Printable ASCII character
        return key;
    switch (key)
    {
        case GLFW_KEY_BACKSPACE: return KEY_BACKSPACE;
        case GLFW_KEY_TAB: return KEY_TAB;
        case GLFW_KEY_ENTER: return KEY_RETURN;
        case GLFW_KEY_PAUSE: return KEY_PAUSE;
        case GLFW_KEY_ESCAPE: return KEY_ESCAPE;
        case GLFW_KEY_DELETE: return KEY_DELETE;
        case GLFW_KEY_UP: return KEY_UP;
        case GLFW_KEY_DOWN: return KEY_DOWN;
        case GLFW_KEY_LEFT: return KEY_LEFT;
        case GLFW_KEY_RIGHT: return KEY_RIGHT;
        case GLFW_KEY_INSERT: return KEY_INSERT;
        case GLFW_KEY_HOME: return KEY_HOME;
        case GLFW_KEY_END: return KEY_END;
        case GLFW_KEY_PAGE_UP: return KEY_PAGE_UP;
        case GLFW_KEY_PAGE_DOWN: return KEY_PAGE_DOWN;
    }
    return -1;
}

int Event::translate_glfw_mods(int mods)
{
    int result = 0;
    if (mods & GLFW_MOD_SHIFT)
        result |= MOD_SHIFT;
    if (mods & GLFW_MOD_CONTROL)
        result |= MOD_CTRL;
    if (mods & GLFW_MOD_ALT)
        result |= MOD_ALT;
    return result;
}

int Event::translate_glfw_mouse(int mouse)
{
    switch (mouse)
    {
        case GLFW_MOUSE_BUTTON_LEFT: return MOUSE_LEFT;
        case GLFW_MOUSE_BUTTON_RIGHT: return MOUSE_RIGHT;
        case GLFW_MOUSE_BUTTON_MIDDLE: return MOUSE_MIDDLE;
    }
    return -1;
}

} // namespace kletch
