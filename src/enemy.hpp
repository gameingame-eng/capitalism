#pragma once
#include "assets.hpp"
#include "global.hpp"
#include "raylib.h"

class Enemy {
public:
  Rectangle rect = {0, 0, MAP_TILE_SIZE, MAP_TILE_SIZE};
  int anim_frame = 0, anim_frame_real = 0, anim_frame_real_max = 0, facing = 0;

  Enemy(Rectangle rect) : rect(rect) {};

  void draw() {
    Rectangle source = {32.0f * anim_frame_real, 32.0f * (facing + 4), 32, 32};
    DrawTexturePro(characters, source, rect, {0, 0}, 0.0f, WHITE);
  }
};
