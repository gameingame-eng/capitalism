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
      }
    }

    const int ntxs[2] = {
        (int)floor((int)(hitbox.x + vel.x * dt) / (int)MAP_TILE_SIZE) + 50,
        (int)floor((int)(hitbox.x + vel.x * dt + hitbox.width) /
                   (int)MAP_TILE_SIZE) +
            50};
    const int ntys[2] = {
        (int)floor((int)(hitbox.y + vel.y * dt) / (int)MAP_TILE_SIZE) + 50,
        (int)floor((int)(hitbox.y + vel.y * dt + hitbox.height) /
                   (int)MAP_TILE_SIZE) +
            50};

    const int ctxs[2] = {
        (int)floor((int)(hitbox.x) / (int)MAP_TILE_SIZE) + 50,
        (int)floor((int)(hitbox.x + hitbox.width) / (int)MAP_TILE_SIZE) + 50};
    const int ctys[2] = {
        (int)floor((int)(hitbox.y) / (int)MAP_TILE_SIZE) + 50,
        (int)floor((int)(hitbox.y + hitbox.height) / (int)MAP_TILE_SIZE) + 50};

    auto iswall = [&](int x, int y) {
      return map[y][x].type == MAP_TILE_TYPE_WALL;
    };

    if (!(iswall(ntxs[0], ctys[0]) || iswall(ntxs[0], ctys[1]) ||
          iswall(ntxs[1], ctys[0]) || iswall(ntxs[1], ctys[1]))) {
      rect.x += vel.x * dt;
    }
    if (!(iswall(ctxs[0], ntys[0]) || iswall(ctxs[0], ntys[1]) ||
          iswall(ctxs[1], ntys[0]) || iswall(ctxs[1], ntys[1]))) {
      rect.y += vel.y * dt;
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

  void draw() {
    DrawTexturePro(
        characters,
        GetPlayerSpriteSource(this->state, this->anim_frame, this->facing),
        this->rect, {0, 0}, 0.0f, WHITE);
  }
};
