#pragma once

#if defined(GLFW)
#include "glex/common/gl.h"
enum KeyCode {
    A = GLFW_KEY_A,
    B = GLFW_KEY_B,
    C = GLFW_KEY_C,
    D = GLFW_KEY_D,
    E = GLFW_KEY_E,
    F = GLFW_KEY_F,
    G = GLFW_KEY_G,
    H = GLFW_KEY_H,
    I = GLFW_KEY_I,
    J = GLFW_KEY_J,
    K = GLFW_KEY_K,
    L = GLFW_KEY_L,
    M = GLFW_KEY_M,
    N = GLFW_KEY_N,
    O = GLFW_KEY_O,
    P = GLFW_KEY_P,
    Q = GLFW_KEY_Q,
    R = GLFW_KEY_R,
    S = GLFW_KEY_S,
    T = GLFW_KEY_T,
    U = GLFW_KEY_U,
    V = GLFW_KEY_V,
    W = GLFW_KEY_W,
    X = GLFW_KEY_X,
    Y = GLFW_KEY_Y,
    Z = GLFW_KEY_Z,

    Enter = GLFW_KEY_ENTER,
    Escape = GLFW_KEY_ESCAPE,
    Backspace = GLFW_KEY_BACKSPACE,
    Tab = GLFW_KEY_TAB,
    Space = GLFW_KEY_SPACE
};

#elif defined(DREAMCAST)
#include <dc/maple/keyboard.h>

// TODO: Finish adding all keys
enum KeyCode {
    A = KBD_KEY_A,
    B = KBD_KEY_B,
    C = KBD_KEY_C,
    D = KBD_KEY_D,
    E = KBD_KEY_E,
    F = KBD_KEY_F,
    G = KBD_KEY_G,
    H = KBD_KEY_H,
    I = KBD_KEY_I,
    J = KBD_KEY_J,
    K = KBD_KEY_K,
    L = KBD_KEY_L,
    M = KBD_KEY_M,
    N = KBD_KEY_N,
    O = KBD_KEY_O,
    P = KBD_KEY_P,
    Q = KBD_KEY_Q,
    R = KBD_KEY_R,
    S = KBD_KEY_S,
    T = KBD_KEY_T,
    U = KBD_KEY_U,
    V = KBD_KEY_V,
    W = KBD_KEY_W,
    X = KBD_KEY_X,
    Y = KBD_KEY_Y,
    Z = KBD_KEY_Z,
    
    Enter = KBD_KEY_ENTER,
    Escape = KBD_KEY_ESCAPE,
    Backspace = KBD_KEY_BACKSPACE,
    Tab = KBD_KEY_TAB,
    Space = KBD_KEY_SPACE
};

#endif