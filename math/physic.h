#ifndef __PHYSIC__
#define __PHYSIC__
#include "../level/tile.h"
#include "../structs/aabb.h"
#include "../structs/minkowski.h"
#include "../structs/player.h"
#include "vector.h"
#include <math.h>
#define MAX_POINTS 16

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

/* perpendicular that points toward 'to' */
static inline vector perpToward(vector edge, vector to) {
  /* left-hand perp of edge */
  vector p = (vector){-edge.y, edge.x};
  if (dot(p, to) < 0) { /* wrong side? flip it */
    p.x = -p.x;
    p.y = -p.y;
  }
  return p;
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
  float best_dot = dot(vertices[0], dir); // Start with first vertex
  vector best = vertices[0];

  for (int x = 1; x < 4; x++) {
    float d = dot(vertices[x], dir);
    if (d > best_dot) {
      best = vertices[x];
      best_dot = d;
    }
  }
  return best;
}

vector support(vector rect1_vertices[4], vector rect2_vertices[4], vector dir) {
  vector a = furtherst_point(rect1_vertices, dir);

  vector neg_dir = dir;
  scale(&neg_dir, -1.f);

  vector b = furtherst_point(rect2_vertices, neg_dir);
  return subtract(a, b);
}

static bool handleLine(Stack *simplex, vector *dir) {
  /* A = last pushed, B = previous */
  vector A = simplex->data[simplex->length - 1];
  vector B = simplex->data[simplex->length - 2];

  vector AB = subtract(B, A);
  vector AO = negate(A);

  *dir = perpToward(AB, AO);
  return false; /* still need another point */
}

static bool handleTriangle(Stack *simplex, vector *dir) {
  vector A = simplex->data[2];
  vector B = simplex->data[1];
  vector C = simplex->data[0];

  vector AO = negate(A);
  vector AB = subtract(B, A);
  vector AC = subtract(C, A);

  /* test edge AB */
  vector ABperp = perpToward(AB, AC);
  if (dot(ABperp, AO) > 0) {
    /* keep A,B  (drop C) */
    simplex->data[0] = B;
    simplex->data[1] = A;
    simplex->length = 2;
    *dir = ABperp;
    return false;
  }

  /* test edge AC */
  vector ACperp = perpToward(AC, AB);
  if (dot(ACperp, AO) > 0) {
    /* keep A,C  (drop B) */
    simplex->data[0] = C;
    simplex->data[1] = A;
    simplex->length = 2;
    *dir = ACperp;
    return false;
  }

  /* origin is inside the triangle */
  return true;
}

static bool handleSimplex(Stack *simplex, vector *dir) {
  return simplex->length == 2 ? handleLine(simplex, dir)
                              : handleTriangle(simplex, dir);
}

bool gjk_collision(Player *pl, Tile *ti, Convex *out) {
  vector dir = (vector){1, 0};
  Stack simplex;
  create_stack(&simplex);

  /* --- first point --- */
  push(support(pl->vertices, ti->vertices, dir), &simplex);
  dir = negate(simplex.data[0]);

  for (int iter = 0; iter < 40; ++iter) {
    /* 1. get next support point */
    vector P = support(pl->vertices, ti->vertices, dir);

    /* 2. fail-fast gate */
    if (dot(P, dir) <= 0) {
      free(simplex.data);
      return false;
    }

    /* 3. skip exact duplicates (precision glitches) */
    bool dup = false;
    for (int i = 0; i < simplex.length; i++)
      if (equal(simplex.data[i], P)) {
        dup = true;
        break;
      }
    if (dup)
      continue;

    /* 4. add & test simplex */
    push(P, &simplex);
    if (handleSimplex(&simplex, &dir)) {
      /* success – copy simplex for EPA */
      out->points = *copy_stack(&simplex);
      out->side_counter = out->points.length;
      return true;
    }
  }
  free(simplex.data);
  return false; /* exceeded iteration budget */
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
      printf("Edge norma; %f %f depth %f \n", edge.normal.x, edge.normal.y,
             edge.depth);
      return true;
    }
    int insert_index = edge.second;
    add_value(&convex.points, sup, insert_index);
  }
  return false;
}

void check_collision_gjk(Player *player, Tile **tiles, int tiles_count) {
  bool grounded = false;
  vector player_min = get_min(player->vertices);
  vector player_max = get_max(player->vertices);

  for (int x = 0; x < tiles_count; x++) {
    Tile *tile = tiles[x];

    Convex *convex = malloc(sizeof(Convex));
    create_stack(&convex->points);
    if (gjk_collision(player, tile, convex)) {
      printf("Collision\n");
      Edge edge;
      if (epa(player, tile, *convex, &edge)) {
        printf("EPA\n");
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

bool aabb_overlap_x(AABB a, AABB b) {

  return !(a.max.x < b.min.x || a.min.x > b.max.x);
}

bool aabb_overlap_y(AABB a, AABB b) {
  return !(a.max.y < b.min.y || a.min.y > b.max.y);
}

void check_collision_player_tile_x(Player *player, Tile *tile) {

  if (!aabb_overlap_y(player->aabb, tile->aabb))
    return; // only resolve if overlapping in Y
  if (!aabb_overlap_x(player->aabb, tile->aabb))
    return;

  update_aabb_player(player);
  if (player->velocity.x > 0.f) {
    printf("Colliding on X (right)\n");
    // player->position.x -= player->velocity.x * dt;
    // player->velocity.x = 0.f;
    player->position.x = (tile->aabb.min.x - player->aabb.min.x) * 0.5f;

  } else if (player->velocity.x < 0.f) {
    printf("Colliding on X (left)\n");
    // player->position.x -= player->velocity.x * dt;
    // player->velocity.x = 0.f;

  }
}

void check_collsion_player_tile_y(Player *player, Tile *tile,
                                  bool *grounded_out) {
  if (!aabb_overlap_x(player->aabb, tile->aabb))
    return;
  if (!aabb_overlap_y(player->aabb, tile->aabb))
    return;

  float playerHalfH = (player->aabb.max.y - player->aabb.min.y) * 0.5f;

  update_aabb_player(player);
  if (player->velocity.y > 0.f) {
    // Moving down — landed on tile
    player->position.y = tile->aabb.min.y - playerHalfH;
    player->velocity.y = 0.f;
    player->onGround = true;
    if (grounded_out)
      *grounded_out = true;
  } else if (player->velocity.y < 0.f) {
    // Moving up — hit bottom of tile above
    player->position.y = tile->aabb.max.y + playerHalfH;
    player->velocity.y = 0.f;
  }
}


// Call this each frame, after you’ve set p->velocity by input+gravity,
// and before you render.
  // 1) Precompute half-sizes once
void player_ground_collision(Player *p, Tile **tiles, int tileCount, float dt) {
    // 1) Precompute half-sizes once
    float halfW = p->size.x * 0.5f;
    float halfH = p->size.y * 0.5f;

    // ----- MOVE ALONG X -----
    p->position.x += p->velocity.x * dt;

    for (int i = 0; i < tileCount; i++) {
        Tile *t = tiles[i];

        float tHalfW = t->size.x * 0.5f;
        float tHalfH = t->size.y * 0.5f;

        // 2) Compute AABB extents for player and tile
        float pL = p->position.x - halfW;
        float pR = p->position.x + halfW;
        float pT = p->position.y - halfH;
        float pB = p->position.y + halfH;

        float tL = t->position.x - tHalfW;
        float tR = t->position.x + tHalfW;
        float tT = t->position.y - tHalfH;
        float tB = t->position.y + tHalfH;

        // 3) Broad‐phase overlap test
        if (pR <= tL || pL >= tR || pB <= tT || pT >= tB)
            continue;

        // 4) Compute penetration depths on each axis
        float overlapX = (p->velocity.x > 0) 
            ? (pR - tL)   // moving right: how far past tile’s left side?
            : (tR - pL);  // moving left: how far past tile’s right side?

        // Y‐penetration even though we didn’t move Y yet—used to compare depths:
        float overlapY = (p->velocity.y > 0) 
            ? (pB - tT)   // moving down: how far into tile’s top?
            : (tB - pT);  // moving up: into tile’s bottom?

        // 5) Only resolve X if it’s the smaller penetration
        if (overlapX < overlapY) {
            if (p->velocity.x > 0) {
                // push player back to the left of the tile
                p->position.x -= overlapX;
            } else if (p->velocity.x < 0) {
                // push player back to the right of the tile
                p->position.x += overlapX;
            }
            // damp a bit of vertical momentum on a glancing hit
            p->velocity.y *= 0.8f;
            p->velocity.x = 0;
        }
    }

    // ----- MOVE ALONG Y -----
    p->position.y += p->velocity.y * dt;
    bool grounded = false;

    for (int i = 0; i < tileCount; i++) {
        Tile *t = tiles[i];
        float tHalfW = t->size.x * 0.5f;
        float tHalfH = t->size.y * 0.5f;

        float pL = p->position.x - halfW;
        float pR = p->position.x + halfW;
        float pT = p->position.y - halfH;
        float pB = p->position.y + halfH;

        float tL = t->position.x - tHalfW;
        float tR = t->position.x + tHalfW;
        float tT = t->position.y - tHalfH;
        float tB = t->position.y + tHalfH;

        if (pR <= tL || pL >= tR || pB <= tT || pT >= tB)
            continue;

        float overlapX = (p->velocity.x > 0) 
            ? (pR - tL) 
            : (tR - pL);

        float overlapY = (p->velocity.y > 0) 
            ? (pB - tT) 
            : (tB - pT);

        // Only resolve Y if it’s the smaller penetration
        if (overlapY < overlapX) {
            if (p->velocity.y > 0) {
                // landing on top of tile
                p->position.y -= overlapY;
                p->velocity.y = 0;
                grounded = true;
                p->state = IDLE;
            } else if (p->velocity.y < 0) {
                // hitting ceiling
                p->position.y += overlapY;
                p->velocity.y = 0;
            }
        }
    }

    // 6) Update grounded state
    p->onGround = grounded;
}

void circle_rect_collision(vector circle_position,vector rect_position,vector rect_size, int radius){

  float rect_left = rect_position.x - rect_size.x/2.f;
  float rect_right = rect_position.x + rect_size.x /2.f;
  float rect_top = rect_position.y - rect_size.y / 2.f;
  float rect_bot = rect_position.y + rect_size.y / 2.f;

  float closestX = clamp((float)radius,rect_left,rect_right);
  float closestY = clamp((float) radius,rect_top,rect_bot);

  float dx = circle_position.x  - closestX;
  float dy = circle_position.y - closestY;

  float dist_sq = dx * dx + dy*dy;

  if(dist_sq < radius * radius){
    printf("Circle rect collision\n");
  }
   


}

#endif
