#ifndef __PHYSIC__
#define __PHYSIC__
#include "../level/tile.h"
#include "../structs/minkowski.h"
#include "../structs/player.h"
#include "vector.h"
#define MAX_POINTS 8

static const float GRAVITY = 775.f;
static const int TILE_SIZE = 24;
void applyGravity(Player *player, float dt) {

  if (!player->onGround) {
    player->velocity.y += GRAVITY * dt;
  }
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

  return subtract(furtherst_point(rect1_vertices, dir),
                  furtherst_point(rect2_vertices, dir));
}

bool line_case(Convex *convex, vector *direction) {
  // Let A = simplex[1] (newest), B = simplex[0] (older)

  vector AB = subtract(convex->points[0], convex->points[1]);
  vector AO = subtract((vector){0.f, 0.f, 0.f}, convex->points[1]);

  // Perpendicular to AB, toward origin
  vector ABperp = tripleCross2D(AB, AO);
  // Update direction
  *direction = ABperp;

  // Keep only A and B in the simplex

  return false;
}
bool triangle_case(Convex *convex, vector *direction) {
  vector A = convex->points[2]; // newest
  vector B = convex->points[1];
  vector C = convex->points[0];
  vector AO = subtract((vector){0.f, 0.f}, A);
  vector AB = subtract(B, A);
  vector AC = subtract(C, A);

  vector ABperp = tripleCross2D(AB, AO);
  vector ACperp = tripleCross2D(AC, AO);

  if (dot(ABperp, AO) > 0.f) {
    // Origin is outside AB
    convex->points[0] = B;
    convex->points[1] = A;
    convex->side_counter--;
    *direction = ABperp;
    return false;
  }

  if (dot(ACperp, AO) > 0.f) {
    // Origin is outside AC
    convex->points[0] = C;
    convex->points[1] = A;
    convex->side_counter--;
    *direction = ACperp;
    return false;
  }

  // Origin is inside triangle
  return true;
}

bool handle_simplex(Convex *convex, vector *direction) {
  if (convex->side_counter <= 2) {
    return line_case(convex, direction);
  }
  return triangle_case(convex, direction);
}

bool gjk_collision(Player *player, Tile *tiles, Convex *out_convex) {
  Convex convex;
  create_convex(&convex);

  vector origin = {0.f, 0.f};

  vector direction = (vector){1.f, 0.f};
  convex.points[convex.side_counter++] =
      support(player->vertices, tiles->vertices, direction);

  direction = subtract(origin, convex.points[0]);

  int iterations = 0;
  int max_iterations = 25;

  while (true) {
    if (++iterations > max_iterations)
      return false;

    vector B = support(player->vertices, tiles->vertices, direction);
    printf("support() = (%.2f, %.2f), dir = (%.2f, %.2f)\n", B.x, B.y,
           direction.x, direction.y);

    printf("player vertices:\n");
    for (int i = 0; i < 4; i++) {
      printf("  V%d: (%.2f, %.2f)\n", i, player->vertices[i].x,
             player->vertices[i].y);
    }

    if (dot(B, direction) < 0) {
      return false;
    }

    convex.points[convex.side_counter++] = B;

    if (convex.side_counter > 3) {
      // rotate points to keep last 3
      convex.points[0] = convex.points[1];
      convex.points[1] = convex.points[2];
      convex.points[2] = B;
      convex.side_counter = 3;
    }

    if (handle_simplex(&convex, &direction)) {
      // Ensure triangle for EPA (if only 2 points exist, generate 3rd)
      if (convex.side_counter == 2) {
        vector A = convex.points[1];
        vector B = convex.points[0];
        vector AB = subtract(B, A);
        vector perp = (vector){-AB.y, AB.x};
        normalize(&perp);

        vector C = support(player->vertices, tiles->vertices, perp);
        convex.points[2] = C;
        convex.side_counter = 3;
      }

      printf("Final GJK simplex (%d points):\n", convex.side_counter);
      for (int i = 0; i < convex.side_counter; i++) {
        printf("  P%d: (%.2f, %.2f)\n", i, convex.points[i].x,
               convex.points[i].y);
      }

      memcpy(out_convex, &convex, sizeof(Convex));
      return true;
    }
  }
}

Edge find_closest(Convex convex) {
  Edge edge = {0};
  float closest = __FLT_MAX__;

  for (int i = 0; i < convex.side_counter; i++) {
    int j = (i + 1) % convex.side_counter;
    vector a = convex.points[i];
    vector b = convex.points[j];

    vector ab = subtract(b, a);
    vector normal = (vector){-ab.y, ab.x}; // Perpendicular
    normalize(&normal);

    float distance = dot(normal, a);

    // Flip normal to point outward if needed
    // if (distance < 0.f) {
    //   normal.x = -normal.x;
    //   normal.y = -normal.y;
    //   distance = -distance;
    // }

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
    if (convex.side_counter >= MAX_POINTS - 1)
      break; // prevent overflow

    Edge edge = find_closest(convex);

    vector sup = support(player->vertices, tile->vertices, edge.normal);
    float d = dot(sup, edge.normal);

    if ((d - edge.depth) < tolerance) {
      out_edge->depth = d;
      printf("Out %f %f\n", edge.normal.x, edge.normal.y);
      out_edge->normal = edge.normal;
      return true;
    }

    int insert_index = edge.second;
    for (int i = convex.side_counter; i > insert_index; --i) {
      convex.points[i] = convex.points[i - 1];
    }

    convex.points[insert_index] = sup;
    convex.side_counter++;
  }

  return false;
}

void check_collision_gjk(Player *player, Tile **tiles, int tiles_count) {
  bool grounded = false;

  for (int x = 0; x < tiles_count; x++) {
    Convex *convex = malloc(sizeof(Convex));
    if (gjk_collision(player, tiles[x], convex)) {
      Edge edge;
      if (epa(player, tiles[x], *convex, &edge)) {

        // Debug: print position before correction
        printf("Normal and depth %f %f %f\n", edge.normal.x, edge.normal.y,
               edge.depth);
        // Clamp max correction depth
        float depth = fminf(edge.depth, 1.0f);
        float bias = 0.01f; // Stronger bias for stable separation

        // Snap normal to dominant axis
        vector snapped_normal = edge.normal;
        // if (fabsf(edge.normal.x) > fabsf(edge.normal.y)) {
        //   // Dominant X axis
        //   snapped_normal.x = edge.normal.x > 0.f ? 1.f : -1.f;
        //   snapped_normal.y = 0.f;
        // } else {
        //   // Dominant Y axis
        //   snapped_normal.x = 0.f;
        //   snapped_normal.y = edge.normal.y > 0.f ? 1.f : -1.f;
        // }

        // Apply correction
        vector correction = snapped_normal;
        scale(&correction, edge.depth + bias);
        addToVector(&player->position, correction);

        // If pushed upward, mark as grounded
        if (snapped_normal.y < 0.f) {
          grounded = true;
          player->velocity.y = 0.f;
        }
        if (snapped_normal.x < 0.f) {
          player->velocity.x = 0.f;
        }
      }
    }
    free(convex);
  }

  player->onGround = grounded;
}

// deprecated
void player_ground_collision(Player *p, Tile **tiles, int tileCount, float dt) {
  // 1) Precompute half-sizes once
  float halfW = p->size.x * 0.5f;
  float halfH = p->size.y * 0.5f;

  // ----- MOVE ALONG X -----
  // 2) Advance X position by velocity.x * dt
  p->position.x += p->velocity.x * dt;

  // 3) For each tile, check X overlap and resolve
  for (int i = 0; i < tileCount; i++) {
    Tile *t = tiles[i];
    float tHalfW = t->size.x * 0.5f;
    float tHalfH = t->size.y * 0.5f;

    // 4) Compute AABB edges
    float pL = p->position.x - halfW;
    float pR = p->position.x + halfW;
    float pT = p->position.y - halfH;
    float pB = p->position.y + halfH;

    float tL = t->position.x - tHalfW;
    float tR = t->position.x + tHalfW;
    float tT = t->position.y - tHalfH;
    float tB = t->position.y + tHalfH;

    // 5) Test for overlap on both axes
    if (pR > tL && pL < tR && pB > tT && pT < tB) {
      // 6) Resolve X penetration
      if (p->velocity.x > 0) {
        // moving right → push back to left side of tile
        p->velocity.y *= .8f;
        p->position.x = tL - halfW;
      } else if (p->velocity.x < 0) {
        // moving left → push back to right side of tile
        p->velocity.y *= .8f;
        p->position.x = tR + halfW;
      }
      p->velocity.x = 0;
    }
  }

  // ----- MOVE ALONG Y -----
  // 7) Advance Y position by velocity.y * dt
  p->position.y += p->velocity.y * dt;

  bool grounded = false;
  for (int i = 0; i < tileCount; i++) {
    Tile *t = tiles[i];
    float tHalfW = t->size.x * 0.5f;
    float tHalfH = t->size.y * 0.5f;

    // 8) Recompute AABB edges because p->position.y changed
    float pL = p->position.x - halfW;
    float pR = p->position.x + halfW;
    float pT = p->position.y - halfH;
    float pB = p->position.y + halfH;

    float tL = t->position.x - tHalfW;
    float tR = t->position.x + tHalfW;
    float tT = t->position.y - tHalfH;
    float tB = t->position.y + tHalfH;

    // 9) Test for overlap
    if (pR > tL && pL < tR && pB > tT && pT < tB) {
      // 10) Resolve Y penetration
      if (p->velocity.y > 0) {
        // moving down → we hit the top of the tile
        p->position.y = tT - halfH;
        p->velocity.y = 0;
        grounded = true;
      } else if (p->velocity.y < 0) {
        // moving up → we hit the bottom of the tile above us
        p->position.y = tB + halfH;
        p->velocity.y = 0;
      }
    }
  }

  // 11) Update grounded flag
  p->onGround = grounded;
}

#endif