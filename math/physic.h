#ifndef __PHYSIC__
#define __PHYSIC__
#include "../level/tile.h"
#include "../structs/minkowski.h"
#include "../structs/player.h"
#include "vector.h"
#include <math.h>
#define MAX_POINTS 8

static const float GRAVITY = 775.f;
static const int TILE_SIZE = 24;

bool equal(vector a, vector b) {
  const float eps = 0.001f;
  return fabsf(a.x - b.x) < eps && fabsf(a.y - b.y) < eps;
}

void applyGravity(Player *player, float dt) {
  if (!player->onGround) {
    player->velocity.y += GRAVITY * dt;
  }
}

bool aabb_overlap(vector min1, vector max1, vector min2, vector max2) {
  return (min1.x <= max2.x && max1.x >= min2.x) &&
         (min1.y <= max2.y && max1.y >= min2.y);
}

vector get_min(vector vertices[4]) {
  vector min = vertices[0];
  for (int i = 1; i < 4; i++) {
    if (vertices[i].x < min.x)
      min.x = vertices[i].x;
    if (vertices[i].y < min.y)
      min.y = vertices[i].y;
  }
  return min;
}

vector get_max(vector vertices[4]) {
  vector max = vertices[0];
  for (int i = 1; i < 4; i++) {
    if (vertices[i].x > max.x)
      max.x = vertices[i].x;
    if (vertices[i].y > max.y)
      max.y = vertices[i].y;
  }
  return max;
}

vector furtherst_point(vector vertices[4], vector dir) {
  float best_dot = -__FLT_MAX__;
  vector best;
  for (int x = 0; x < 4; x++) {
    float d = dot(vertices[x], dir);
    if (d > best_dot) {
      best = vertices[x];
      best_dot = d;
    }
  }
  return best;
}

vector support(vector rect1_vertices[4], vector rect2_vertices[4], vector dir) {
  vector furthest_point = furtherst_point(rect1_vertices, dir);
  scale(&dir, -1.f);
  vector tile_point = furtherst_point(rect2_vertices, dir);
  return subtract(furthest_point, tile_point);
}

bool line_case(Convex *convex, vector *direction) {

  Stack *stack = copy_stack(&convex->points);

  vector A = pop(stack);
  vector B = pop(stack);

  vector AB = subtract(A, B);
  vector AO = subtract((vector){0.f, 0.f}, A);
  *direction = tripleCross2D(AB, AO);
  // free(stack->data);
  free(stack->data);
  free(stack);

  return false;
}

bool triangle_case(Convex *convex, vector *direction) {
  Stack *conv_stack = copy_stack(&convex->points);
  if (!conv_stack) return false;

  Stack res = {0};
  res.data = malloc(sizeof(vector) * convex->points.capacity);
  res.capacity = convex->points.capacity;
  res.length = 0;

  vector C = pop(conv_stack);
  vector B = pop(conv_stack);
  vector A = pop(conv_stack);
  vector AO = subtract((vector){0.f, 0.f}, A);
  vector AB = subtract(B, A);
  vector AC = subtract(C, A);

  vector ABperp = tripleCross2D(AB, AO);
  vector ACperp = tripleCross2D(AC, AO);

  if (dot(ABperp, AO) > 0.f) {
    push(B, &res);
    push(A, &res);
    *direction = ABperp;
    normalize(direction);

    // Replace old stack with new one
    free(convex->points.data);
    convex->points = res;

    free(conv_stack->data);
    free(conv_stack);
    return false;
  }

  if (dot(ACperp, AO) > 0.f) {
    push(C, &res);
    push(A, &res);
    *direction = ACperp;

    normalize(direction);
    free(convex->points.data);
    convex->points = res;

    free(conv_stack->data);
    free(conv_stack);
    return false;
  }

  free(res.data); // wasn't used
  free(conv_stack->data);
  free(conv_stack);
  return true;
}



bool handle_simplex(Convex *convex, vector *direction) {
  if (convex->points.length <= 2) {
    return line_case(convex, direction);
  }
  return triangle_case(convex, direction);
}

bool gjk_collision(Player *player, Tile *tiles, Convex *out_convex) {
  Convex convex;
  create_convex(&convex);
  vector origin = {0.f, 0.f};
  vector direction = (vector){1.f, 0.f};
   vector A = support(player->vertices, tiles->vertices, direction);
  push(A,&convex.points );
  direction = subtract(origin,A);
   normalize(&direction);
  int iterations = 0;
  const int max_iterations = 25;

  while (true) {
    if (++iterations > max_iterations)
      return false;
    vector B = support(player->vertices, tiles->vertices, direction);

    for (int i = 0; i < convex.side_counter; ++i) {
      if (equal(convex.points.data[i], B))
        return false; // prevent duplicates
    }
    if (dot(B, direction) < 0)
      return false;

    push(B,&convex.points);

    if (convex.points.length > 3) {

      convex.points.data[0] = convex.points.data[1];
      convex.points.data[1] = convex.points.data[2];
      convex.points.data[2] = B;
      convex.points.length = 3;
      convex.side_counter = 3;
    }
    if (handle_simplex(&convex, &direction)) {
     
 
      Stack* res = copy_stack(&convex.points);
        
      out_convex->points = *res;
       out_convex->side_counter = convex.side_counter;
       return true;
    }
  }

  free(convex.points.data);
  return false;
}

Edge find_closest(Convex convex) {
  Edge edge = {0};
  float closest = __FLT_MAX__;
  for (int i = 0; i < convex.points.length; i++) {
    int j = (i + 1) % convex.points.length;
    vector a = convex.points.data[i];
    vector b = convex.points.data[j];
    vector ab = subtract(b, a);
    vector normal = (vector){-ab.y, ab.x};
    normalize(&normal);
    float distance = dot(normal, a);
    if (distance < closest) {
      closest = distance;
      edge.first = i;
      edge.second = j;
      edge.normal = normal;
      edge.depth = distance;
    }
  }
  return edge;
}

bool epa(Player *player, Tile *tile, Convex convex, Edge *out_edge) {
  const int max_iterations = 16;
  const float tolerance = 0.001f;
  for (int iter = 0; iter < max_iterations; iter++) {
    if (convex.points.length >= MAX_POINTS - 1)
      break;
    Edge edge = find_closest(convex);
    vector sup = support(player->vertices, tile->vertices, edge.normal);
    float d = dot(sup, edge.normal);
    if ((d - edge.depth) < tolerance) {
      out_edge->depth = d;
      out_edge->normal = edge.normal;
      return true;
    }
    int insert_index = edge.second;
     add_value(&convex.points,sup,insert_index);
  }
  return false;
}

void check_collision_gjk(Player *player, Tile **tiles, int tiles_count) {
  bool grounded = false;
  vector player_min = get_min(player->vertices);
  vector player_max = get_max(player->vertices);

  for (int x = 0; x < tiles_count; x++) {
    Tile *tile = tiles[x];
    vector tile_min = get_min(tile->vertices);
    vector tile_max = get_max(tile->vertices);
    if (!aabb_overlap(player_min, player_max, tile_min, tile_max))
      continue;

    Convex *convex = malloc(sizeof(Convex));
    create_stack(&convex->points);
    if (gjk_collision(player, tiles[x], convex)) {
      Edge edge;
      if (epa(player, tiles[x], *convex, &edge)) {
        float depth = fminf(edge.depth, 1.0f);
        float bias = 0.001f;
        vector correction = edge.normal;
        scale(&correction, depth + bias);
        addToVector(&player->position, correction);
        if (edge.normal.y < 0.f) {
          grounded = true;
          player->velocity.y = 0.f;
        }
        if (edge.normal.x < 0.f) {
          player->velocity.x = 0.f;
        }
      }
    }
    free(convex->points.data);
    free(convex);
  }
  player->onGround = grounded;
}

#endif
