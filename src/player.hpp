#pragma once
#include "assets.hpp"
#include "enemy.hpp"
#include "gamestate.hpp"
#include "global.hpp"
#include "map.hpp"
#include "raylib.h"
#include <cmath>
#include <vector>

class Player {
public:
  Rectangle rect = {0, 0, MAP_TILE_SIZE, MAP_TILE_SIZE};
  Rectangle hitbox = {0, 0, MAP_TILE_SIZE, MAP_TILE_SIZE};

  int facing = 0;
  int anim_frame = 0;
  int anim_frame_real_max = 8;
  int anim_frame_real = 0;
  float speed = 200;
  PlayerSpriteState state;
  Vector2 vel = {0, 0};
  float friction = 200;

  int attackSize = 80;
  Rectangle attackRect = {0, 0, 1, 1};
  int attackSustain = 0;
  const int attackSustainMax = 30;

  void update(MapTile map[100][100], float dt, Camera2D &gamecam,
              GameState &state, std::vector<Enemy> &enemies) {
    this->hitbox = {this->rect.x + ((this->rect.width / 32.0f) * 10),
                    this->rect.y + ((this->rect.height / 32.0f) * 4),
                    this->rect.width - ((this->rect.width / 32.0f) * 20),
                    this->rect.height - ((this->rect.height / 32.0f) * 8)};

    this->state = PLAYER_SPRITE_STATE_IDLE;

    if (abs(vel.x) >= friction)
      vel.x -= vel.x >= 0 ? friction : -friction;
    else
      vel.x = 0;
    if (abs(vel.y) >= friction)
      vel.y -= vel.y >= 0 ? friction : -friction;
    else
      vel.y = 0;

    attack(enemies);

    if (IsKeyDown(KEY_A)) {
      vel.x = -this->speed;
      this->facing = 1;
      this->state = PLAYER_SPRITE_STATE_WALK;
    } else if (IsKeyDown(KEY_D)) {
      this->facing = 2;
      vel.x = this->speed;
      this->state = PLAYER_SPRITE_STATE_WALK;
    }
    if (IsKeyDown(KEY_W)) {
      vel.y = -speed;
      this->facing = 3;
      this->state = PLAYER_SPRITE_STATE_WALK;
    } else if (IsKeyDown(KEY_S)) {
      vel.y = speed;
      this->facing = 0;
      this->state = PLAYER_SPRITE_STATE_WALK;
    }

    for (auto &e : enemies) {
      if (CheckCollisionRecs(hitbox, e.rect)) {
        Vector2 v = Vector2Scale(
            Vector2Normalize(Vector2Subtract(
                {hitbox.x - hitbox.width / 2, hitbox.y - hitbox.height / 2},
                {e.rect.x - e.rect.width / 2, e.rect.y - e.rect.height / 2})),
            700);

        vel = v;
        e.vel = Vector2Negate(v);

        state.health--;
      }
    }

    auto tileIdx = [](float worldPos) {
      return (int)floor(worldPos / MAP_TILE_SIZE) + 50;
    };

    auto iswall = [&](int x, int y) {
      x = std::clamp(x, 0, 99);
      y = std::clamp(y, 0, 99);
      if (x < 0 || x >= 100 || y < 0 || y >= 100)
        return true;
      return map[y][x].type == MAP_TILE_TYPE_WALL;
    };

    float nextX = hitbox.x + vel.x * dt;
    int ntxL = tileIdx(nextX);
    int ntxR = tileIdx(nextX + hitbox.width - 1);
    int cyT = tileIdx(hitbox.y);
    int cyB = tileIdx(hitbox.y + hitbox.height - 1);

    if (!(iswall(ntxL, cyT) || iswall(ntxL, cyB) || iswall(ntxR, cyT) ||
          iswall(ntxR, cyB))) {
      rect.x += vel.x * dt;
    } else {
      vel.x = 0;
    }

    float nextY = hitbox.y + vel.y * dt;
    int cxL = tileIdx(hitbox.x);
    int cxR = tileIdx(hitbox.x + hitbox.width - 1);
    int ntyT = tileIdx(nextY);
    int ntyB = tileIdx(nextY + hitbox.height - 1);

    if (!(iswall(cxL, ntyT) || iswall(cxR, ntyT) || iswall(cxL, ntyB) ||
          iswall(cxR, ntyB))) {
      rect.y += vel.y * dt;
    } else {
      vel.y = 0;
    }
    if (std::abs((this->rect.x + this->rect.width / 2) - gamecam.target.x) >
        100) {
      if (this->rect.x > gamecam.target.x)
        gamecam.target.x =
            this->rect.x + this->rect.width / 2 - PLAYER_CAMERA_FOLLOW_MARGIN;
      else
        gamecam.target.x =
            this->rect.x + this->rect.width / 2 + PLAYER_CAMERA_FOLLOW_MARGIN;
    }
    if (std::abs((this->rect.y + this->rect.height / 2) - gamecam.target.y) >
        75) {
      if (this->rect.y > gamecam.target.y)
        gamecam.target.y = this->rect.y + this->rect.height / 2 -
                           PLAYER_CAMERA_FOLLOW_MARGIN * 0.75;
      else
        gamecam.target.y = this->rect.y + this->rect.height / 2 +
                           PLAYER_CAMERA_FOLLOW_MARGIN * 0.75;
    }

    this->anim_frame_real++;
    if (this->anim_frame_real == this->anim_frame_real_max) {
      this->anim_frame = (this->anim_frame + 1) % 2;
      this->anim_frame_real = 0;
    }
  }

  void attack(std::vector<Enemy> &enemies) {
    if (attackSustain > 0) {
      attackSustain--;
      attackRect = {rect.x + rect.width / 2.0f - attackSize / 2.0f,
                    rect.y + rect.height / 2.0f - attackSize / 2.0f,
                    (float)attackSize, (float)attackSize};

      if (facing == 0) {
        attackRect.y += rect.height / 2.0f + attackSize;
        attackRect.width = attackSize * 2;
        attackRect.x -= attackSize / 2.0f;
      }
      if (facing == 1) {
        attackRect.x -= rect.width / 2.0f + attackSize;
        attackRect.height = attackSize * 2;
        attackRect.y -= attackSize / 2.0f;
      }
      if (facing == 2) {
        attackRect.x += rect.width / 2.0f + attackSize;
        attackRect.height = attackSize * 2;
        attackRect.y -= attackSize / 2.0f;
      }
      if (facing == 3) {
        attackRect.y -= rect.height / 2.0f + attackSize;
        attackRect.width = attackSize * 2;
        attackRect.x -= attackSize / 2.0f;
      }

    } else
      attackRect = {0, 0, 0, 0};

    for (auto &e : enemies) {
      if (GetCollisionRec(attackRect, e.rect).width != 0) {
        Vector2 v = Vector2Negate(Vector2Scale(
            Vector2Normalize(Vector2Subtract(
                {hitbox.x - hitbox.width / 2, hitbox.y - hitbox.height / 2},
                {e.rect.x - e.rect.width / 2, e.rect.y - e.rect.height / 2})),
            1600));
        e.vel = v;
      }
    }
    if (IsKeyPressed(KEY_SPACE))
      attackSustain = attackSustainMax;
  }

  void draw() {
    DrawTexturePro(
        characters,
        GetPlayerSpriteSource(this->state, this->anim_frame, this->facing),
        this->rect, {0, 0}, 0.0f, WHITE);
    DrawRectangleRec(attackRect, BLUE);
  }
};
