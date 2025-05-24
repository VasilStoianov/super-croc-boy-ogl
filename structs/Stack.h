#include "../math/vector.h"

#define START_CAPACITY 8

typedef struct Stack {

  vector *data;
  int capacity;
  int length;
} Stack;

void push(vector vec, Stack *stack) {
  if (stack->length == stack->capacity / 2) {
    stack->data =
        (vector *)realloc(stack->data, sizeof(vector) * stack->capacity * 2);
    stack->capacity *= 2;
  }
  stack->data[stack->length++] = vec;
}

vector pop(Stack *stack) {

  vector res = stack->data[0];
  for (int x = 0; x < stack->length; x++) {
    stack->data[x] = stack->data[x + 1];
  }
  stack->length--;

  return res;
}

void create_stack(Stack *stack) {

  stack->data = malloc(sizeof(vector) * 4);
  stack->capacity = START_CAPACITY;
  stack->length = 0;
}

void remove_from_stack(Stack *stack, vector value) {
  bool start_moving = 0;
  for (int x = 0; x < stack->length; x++) {
    vector stack_vec = stack->data[x];
    if (stack_vec.x == value.x && stack_vec.y == value.y) {
      start_moving = 1;
    }

    if (start_moving) {
      stack->data[x] = stack->data[x+1];
    }
  }
}

void add_value(Stack* stack,vector value, int insert_index){
 for (int i = stack->length; i > insert_index; --i) {
    stack->data[i] = stack->data[i - 1];
    }
    stack->data[insert_index] =value;
  stack->length++;
}

Stack* copy_stack(Stack* toCopy) {
  Stack *conv = malloc(sizeof(Stack));
  conv->capacity = toCopy->capacity;
  conv->length = toCopy->length;
  conv->data = malloc(sizeof(vector) * conv->capacity);  
  memcpy(conv->data, toCopy->data, sizeof(vector) * conv->length);  
  return conv;
}

void print_stack(Stack stack){
  for(int x = 0; x<stack.length; x++){
    printf("Stack is:\n");
    printf("x: %f y: %f \n",stack.data[x].x,stack.data[x].y);
  }
}
