#ifndef __game__
#define __game__

#include "glad.h"
#include "level/tile.h"
#include "player.h"
#include "utils.h"
#include "vertices.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "camera.h"

static const float GRAVITY = 775.f;
static const int WORLD[34][25];
static const int TILE_SIZE = 24;

void applyGravity(Player *player, float dt) {

  if (!player->onGround) {
    player->velocity.y += GRAVITY * dt;
  }
}

void configVertices(unsigned int *VBO, unsigned int *VAO, unsigned int *EBO) {

  glGenVertexArrays(1, VAO);
  glGenBuffers(1, VBO);
  glGenBuffers(1, EBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(*VAO);

  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

void draw(unsigned int VAO, unsigned int program_id) {
  glUseProgram(program_id);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

bool collision_check(Player *p, Tile *tile) {

  float halfWidth = p->size.x / 2.0f;
  float halfHeight = p->size.y / 2.0f;

  float playerLeft = p->position.x - halfWidth;
  float playerRight = p->position.x + halfWidth;
  float playerTop = p->position.y + halfHeight;
  float playerBottom = p->position.y - halfHeight;

  float halfWidthTile = tile->size.x / 2.0f;
  float halfHeightTile = tile->size.y / 2.0f;

  float tileLeft = tile->position.x - halfWidthTile;
  float tileRight = tile->position.x + halfWidthTile;
  float tileTop = tile->position.y + halfHeightTile;
  float tileBottom = tile->position.y - halfHeightTile;

  return playerRight > tileLeft && playerLeft < tileRight &&
         playerTop >= tileBottom && playerBottom <= tileTop;
}

void player_groun_collision(Player *p, Tile *tile) {

  if (collision_check(p, tile)) {
    if (p->velocity.y > 0) {
      p->position.y = tile->position.y - p->size.y;
    } else if (p->velocity.y < 0) {
      p->position.y = p->size.y / 2.f + tile->position.y + tile->size.y / 2.f;
      p->velocity.y = 0;
      p->onGround = true;
    }
  } else {

    p->onGround = false;
  }
}

// Call this each frame, after you’ve set p->velocity by input+gravity,
// and before you render.
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

void move_camera(Player* player, Camera* camera,int shaderId){
    float plLeft = player->position.x + player->size.x/2.f;
    printf("%f\n",plLeft);

    if(plLeft > 800.f){
      camera->position.x -= 10.f;
    set_camera(camera,shaderId);
    }

}

#endif
