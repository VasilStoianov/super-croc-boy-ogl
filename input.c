#include "input.h"

static GLFWwindow* window;
static unsigned char previous[GLFW_KEY_LAST] = {0};
static unsigned char current[GLFW_KEY_LAST] = {0};
static KeyState states[GLFW_KEY_LAST];

void input_init(GLFWwindow *win){
    window = win;
}

void input_update(){
  
    for(int key = 0; key<GLFW_KEY_LAST; key++){
        current[key] = glfwGetKey(window,key) == GLFW_PRESS;

        if(current[key]){
            if(!previous[key])
              states[key] = KEY_PRESSED;
          
            else
              states[key] = KEY_HELD;
        }else{
            if(previous[key])
              states[key] = KEY_RELEASED;
              
            else 
              states[key] = KEY_UP;
       }
       previous[key] = current[key];
    }
}

int input_key_pressed(int key){
    return states[key] == KEY_PRESSED;
}

int input_key_held(int key){
    return states[key] == KEY_HELD;
}

KeyState input_get_key_state(int key){
    return states[key];
}