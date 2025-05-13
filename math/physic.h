#ifndef __PHYSIC__
#define __PHYSIC__
#include "vector.h"
#include "../player.h"
#include "../level/tile.h"

static const float GRAVITY = 775.f;
static const int TILE_SIZE = 24;
void applyGravity(Player *player, float dt) {

  if (!player->onGround) {
    player->velocity.y += GRAVITY * dt;
  }
}



vector furtherst_point(vector vertices[4], vector dir) {
  float best_dot = -__FLT_MAX__;
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

  return subtract(furtherst_point(rect1_vertices, dir),
                  furtherst_point(rect2_vertices, dir));
}

bool line_case(vector simplex[3], vector *direction) {
  // Let A = simplex[1] (newest), B = simplex[0] (older)
  vector A = simplex[1];
  vector B = simplex[0];

  vector AB = subtract(B, A);
  vector AO = subtract((vector){0.f, 0.f, 0.f}, A);

  // Perpendicular to AB, toward origin
  vector ABperp = cross(cross(AB, AO), AB);

  // Update direction
  *direction = ABperp;

  // Keep only A and B in the simplex
  simplex[0] = B;
  simplex[1] = A;

  return false;
}
bool triangle_case(vector simplex[3], vector *direction) {
  vector A = simplex[2]; // newest
  vector B = simplex[1];
  vector C = simplex[0];
  vector origin = {0.f, 0.f, 0.f};

  vector AB = subtract(B, A);
  vector AC = subtract(C, A);
  vector AO = subtract(origin, A);

  // Compute perpendiculars to edges AB and AC toward origin
  vector ABperp = cross(cross(AC, AB), AB);
  vector ACperp = cross(cross(AB, AC), AC);

  if (dot(ABperp, AO) > 0.f) {
    // Remove point C → keep [B, A]
    simplex[0] = B;
    simplex[1] = A;
    *direction = ABperp;
    return false;
  }

  if (dot(ACperp, AO) > 0.f) {
    // Remove point B → keep [C, A]
    simplex[0] = C;
    simplex[1] = A;
    *direction = ACperp;
    return false;
  }

  // Origin is inside triangle → collision
  return true;
}

bool handle_simplex(vector simplex[3], vector *direction, int simplex_index) {
  if (simplex_index < 2) {
    return line_case(simplex, direction);
  }
  return triangle_case(simplex, direction);
}

bool gjk_collision(Player *player, Tile *tiles) {
  vector simplex[3] = {0};
  vector origin = {0.f, 0.f, 0.f};
  int simplex_index = 0;

  vector direction = {1.f, 0.f, 0.f};
  simplex[simplex_index] =
      support(player->vertices, tiles->vertices, direction);
  simplex_index++;
  direction = subtract(origin, simplex[0]);
  int iterations = 0;
  int max_iterations = 25;
  while (true) {
    if (++iterations > max_iterations)
      return false;
    vector A = support(player->vertices, tiles->vertices, direction);
    if (dot(A, direction) < 0) {
      return false;
    }

    simplex[simplex_index] = A;
    simplex_index++;
    if (simplex_index > 2)
      simplex_index = 2;
    if (handle_simplex(simplex, &direction, simplex_index)) {
      return true;
    }
  }
}

void check_collision_gjk(Player *player, Tile **tiles, int tiles_count) {

  float halfW = player->size.x * 0.5f;
  float halfH = player->size.y * 0.5f;
 float grounded = false;
  for (int x = 0; x < tiles_count; x++) {
    float tHalfW = tiles[x]->size.x * 0.5f;
    float tHalfH = tiles[x]->size.y * 0.5f;


    float tL = tiles[x]->position.x - tHalfW;
    float tR = tiles[x]->position.x + tHalfW;
    float tT = tiles[x]->position.y - tHalfH;
    float tB = tiles[x]->position.y + tHalfH;

    if (gjk_collision(player, tiles[x])) {

      if (player->velocity.y > 0.f) {
        grounded = true;
        player->position.y = tT - halfH;
        player->velocity.y = 0;
      }
      else if(player->velocity.y<0.f){
        player->position.y = tT + halfH;
        player->velocity.y = 12.f;
      }

      if(player->velocity.x > 0.f){
        player->position.x = tL +halfW;
        player->velocity.x = 0;
      }
      else if(player->velocity.y < 0.f){
        player->position.x = tL + halfW;
        player->velocity.x = 0;
      }


    }
  }
  player->onGround = grounded;
}



//deprecated
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