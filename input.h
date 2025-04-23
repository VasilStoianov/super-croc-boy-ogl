#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

typedef enum {
    KEY_UP,
    KEY_PRESSED,
    KEY_HELD,
    KEY_RELEASED
} KeyState;

void input_init(GLFWwindow *win);
void input_update();
KeyState input_get_key_state(int key);
int input_key_pressed(int key);
int input_key_held(int key);

#endif