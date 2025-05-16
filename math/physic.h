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

bool aabb_overlap(vector min1, vector max1, vector min2, vector max2) {
    return (min1.x <= max2.x && max1.x >= min2.x) &&
           (min1.y <= max2.y && max1.y >= min2.y);
}

vector get_min(vector vertices[4]) {
    vector min = vertices[0];
    for (int i = 1; i < 4; i++) {
        if (vertices[i].x < min.x) min.x = vertices[i].x;
        if (vertices[i].y < min.y) min.y = vertices[i].y;
    }
    return min;
}

vector get_max(vector vertices[4]) {
    vector max = vertices[0];
    for (int i = 1; i < 4; i++) {
        if (vertices[i].x > max.x) max.x = vertices[i].x;
        if (vertices[i].y > max.y) max.y = vertices[i].y;
    }
    return max;
}


bool equal(vector a, vector b) {
    const float eps = 0.001f;
    return fabsf(a.x - b.x) < eps && fabsf(a.y - b.y) < eps;
}

// Furthest point in a given direction from a polygon
vector furthest_point(vector vertices[4], vector dir) {
    float best_dot = -__FLT_MAX__;
    vector best = vertices[0];
    for (int i = 0; i < 4; i++) {
        float d = dot(vertices[i], dir);
        if (d > best_dot) {
            best_dot = d;
            best = vertices[i];
        }
    }
    return best;
}

// Minkowski support function
vector support(vector vertsA[4], vector vertsB[4], vector dir) {
    vector pointA = furthest_point(vertsA, dir);
    vector pointB = furthest_point(vertsB, (vector){ -dir.x, -dir.y });
    return subtract(pointA, pointB);
}

bool line_case(Convex *convex, vector *direction) {
  vector A = convex->points[1]; // Newest point
  vector B = convex->points[0];

  vector AB = subtract(B, A);
  vector AO = subtract((vector){0.f, 0.f}, A);

  vector ABperp = tripleCross2D(AB, AO);

  // Fallback if ABperp is too small
  if (vectorLength(ABperp) < 0.001f) {
    ABperp = (vector){-AB.y, AB.x}; // Perpendicular fallback
  }

  normalize(&ABperp);
  *direction = ABperp;

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
    if (vectorLength(ABperp) < 0.001f) {
      ABperp = (vector){-AB.y, AB.x};
    }
    normalize(&ABperp);
    convex->points[0] = B;
    convex->points[1] = A;
    convex->side_counter--;
    *direction = ABperp;
    return false;
  }

  if (dot(ACperp, AO) > 0.f) {
    if (vectorLength(ACperp) < 0.001f) {
      ACperp = (vector){-AC.y, AC.x};
    }
    normalize(&ACperp);
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

bool is_duplicate(Convex *convex, vector pt) {
    for (int i = 0; i < convex->side_counter; i++) {
        if (equal(convex->points[i], pt)) return true;
    }
    return false;
}


bool gjk_collision(Player *player, Tile *tile, Convex *out_convex) {
    Convex simplex;
    create_convex(&simplex);
    vector dir = (vector){1.f, 0.f};  // Initial direction
    vector a = support(player->vertices, tile->vertices, dir);
    simplex.points[simplex.side_counter++] = a;
    dir = (vector){-a.x, -a.y}; // towards origin

    const int max_iter = 25;
    for (int iter = 0; iter < max_iter; iter++) {
        vector b = support(player->vertices, tile->vertices, dir);

        if (dot(b, dir) < 0.f || is_duplicate(&simplex, b)) {
            return false; // No collision or repeated point
        }

        // Add new point
        if (simplex.side_counter >= MAX_POINTS)
            return false; // Overflow guard

        simplex.points[simplex.side_counter++] = b;

        // Maintain at most 3 points (rotate in)
        if (simplex.side_counter > 3) {
            simplex.points[0] = simplex.points[1];
            simplex.points[1] = simplex.points[2];
            simplex.points[2] = b;
            simplex.side_counter = 3;
        }

        if (handle_simplex(&simplex, &dir)) {
            *out_convex = simplex;
            return true;
        }
    }

    return false;
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
  vector player_min = get_min(player->vertices);
    vector player_max = get_max(player->vertices);
        Tile *tile = tiles[x];
        vector tile_min = get_min(tile->vertices);
        vector tile_max = get_max(tile->vertices);

        if (!aabb_overlap(player_min, player_max, tile_min, tile_max)) {
            continue; // Skip faraway tiles
        }

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