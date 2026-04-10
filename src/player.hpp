#pragma once
#include "assets.hpp"
#include "global.hpp"
#include "map.hpp"
#include "raylib.h"
#include <cmath>

class Player {
public:
  Rectangle rect = {0, 0, MAP_TILE_SIZE, MAP_TILE_SIZE};
  Rectangle hitbox = {0, 0, MAP_TILE_SIZE, MAP_TILE_SIZE};

  int facing = 0;
  int anim_frame = 0;
  int anim_frame_real_max = 10;
  int anim_frame_real = 0;
  float speed = 200;
  PlayerSpriteState state;

  void update(MapTile map[100][100], float dt, Camera2D &gamecam) {
    this->hitbox = {this->rect.x + ((this->rect.width / 32.0f) * 10),
                    this->rect.y + ((this->rect.height / 32.0f) * 4),
                    this->rect.width - ((this->rect.width / 32.0f) * 20),
                    this->rect.height - ((this->rect.height / 32.0f) * 8)};

    this->state = PLAYER_SPRITE_STATE_IDLE;

    if (IsKeyDown(KEY_A)) {
      int npxt =
          (int)floor((this->hitbox.x - this->speed * dt) / MAP_TILE_SIZE) + 50;
      int npyts[2] = {
          (int)floor(this->hitbox.y / MAP_TILE_SIZE) + 50,
          (int)floor((this->hitbox.y + this->hitbox.height) / MAP_TILE_SIZE) +
              50};
      if (map[npyts[0]][npxt].type != MAP_TILE_TYPE_WALL &&
          map[npyts[1]][npxt].type != MAP_TILE_TYPE_WALL) {
        this->rect.x -= this->speed * dt;
        this->facing = 1;
        this->state = PLAYER_SPRITE_STATE_WALK;
      }
    } else if (IsKeyDown(KEY_D)) {
      int npxt =
          (int)floor((this->hitbox.x + this->hitbox.width + this->speed * dt) /
                     MAP_TILE_SIZE) +
          50;
      int npyts[2] = {
          (int)floor(this->hitbox.y / MAP_TILE_SIZE) + 50,
          (int)floor((this->hitbox.y + this->hitbox.height) / MAP_TILE_SIZE) +
              50};
      if (map[npyts[0]][npxt].type != MAP_TILE_TYPE_WALL &&
          map[npyts[1]][npxt].type != MAP_TILE_TYPE_WALL) {
        this->rect.x += this->speed * dt;
        this->facing = 2;
        this->state = PLAYER_SPRITE_STATE_WALK;
      }
    }
    if (IsKeyDown(KEY_W)) {
      int npxts[2] = {
          (int)floor(this->hitbox.x / MAP_TILE_SIZE) + 50,
          (int)floor((this->hitbox.x + this->hitbox.width) / MAP_TILE_SIZE) +
              50};
      int npyt =
          (int)floor((this->hitbox.y - this->speed * dt) / MAP_TILE_SIZE) + 50;
      if (map[npyt][npxts[0]].type != MAP_TILE_TYPE_WALL &&
          map[npyt][npxts[1]].type != MAP_TILE_TYPE_WALL) {
        this->rect.y -= this->speed * dt;
        this->facing = 3;
        this->state = PLAYER_SPRITE_STATE_WALK;
      }
    } else if (IsKeyDown(KEY_S)) {
      int npxts[2] = {
          (int)floor(this->hitbox.x / MAP_TILE_SIZE) + 50,
          (int)floor((this->hitbox.x + this->hitbox.width) / MAP_TILE_SIZE) +
              50};
      int npyt =
          (int)floor((this->hitbox.y + this->hitbox.height + this->speed * dt) /
                     MAP_TILE_SIZE) +
          50;
      if (map[npyt][npxts[0]].type != MAP_TILE_TYPE_WALL &&
          map[npyt][npxts[1]].type != MAP_TILE_TYPE_WALL) {

        this->rect.y += this->speed * dt;
        this->facing = 0;
        this->state = PLAYER_SPRITE_STATE_WALK;
      }
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
