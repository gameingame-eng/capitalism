#pragma once
#include "assets.hpp"
#include "global.hpp"
#include "map.hpp"
#include "raylib.h"
#include <algorithm>
#include <raymath.h>

inline int getSide(int px, int py, int x, int y, int w, int h) {
  const int nx = (px - x) / w; // normalize to [0,1]
  const int ny = (py - y) / h;

  const int d1 = ny - nx;
  const int d2 = ny - (1 - nx);

  if (d1 < 0 && d2 < 0)
    return 4; // top
  if (d1 > 0 && d2 > 0)
    return 1; // bottom
  if (d1 > 0 && d2 < 0)
    return 2; // left
  return 3;   // right
}

class Enemy {
public:
  Rectangle rect = {0, 0, MAP_TILE_SIZE, MAP_TILE_SIZE};
  int anim_frame = 1, anim_frame_real = 0, anim_frame_real_max = 10, facing = 0;
  int anim_dir = 1;
  float speed = 70;
  Vector2 vel = {0, 0};
  float friction = 1600;

  Enemy(Rectangle rect) : rect(rect) {};

  void draw() {
    Rectangle source = {32.0f * anim_frame, (32.0f * 3) + 32.0f * facing, 32,
                        32};
    DrawTexturePro(characters, source, rect, {0, 0}, 0.0f, WHITE);
  }

  void update(float dt, Vector2 playercenter, MapTile map[100][100]) {
    if (abs(vel.x) >= friction * dt) {
      vel.x -= vel.x >= 0 ? friction * dt : -friction * dt;
    } else
      vel.x = 0;

    if (abs(vel.y) >= friction * dt) {
      vel.y -= vel.y >= 0 ? friction * dt : -friction * dt;
    } else
      vel.y = 0;

    anim_frame_real++;
    if (anim_frame_real >= anim_frame_real_max) {
      anim_frame_real = 0;
      anim_frame += anim_dir;
      if (anim_frame >= 2 || anim_frame <= 0)
        anim_dir = -anim_dir;
    }

    facing = getSide(playercenter.x, playercenter.y, rect.x, rect.y, rect.width,
                     rect.height);

    Vector2 center = {rect.x + rect.width / 2, rect.y + rect.height / 2};
    Vector2 newpos = Vector2MoveTowards(center, playercenter, speed * dt);

    float newx = newpos.x - rect.width / 2 + vel.x * dt;
    float newy = newpos.y - rect.height / 2 + vel.y * dt;

    auto tileAt = [&](float wx, float wy) -> MapTile & {
      int tx = std::clamp((int)floor(wx / MAP_TILE_SIZE) + 50, 0, 99);
      int ty = std::clamp((int)floor(wy / MAP_TILE_SIZE) + 50, 0, 99);
      return map[ty][tx];
    };

    bool blockX =
        tileAt(newx, rect.y).type == MAP_TILE_TYPE_WALL ||
        tileAt(newx + rect.width, rect.y).type == MAP_TILE_TYPE_WALL ||
        tileAt(newx, rect.y + rect.height - 1).type == MAP_TILE_TYPE_WALL ||
        tileAt(newx + rect.width, rect.y + rect.height - 1).type ==
            MAP_TILE_TYPE_WALL;

    bool blockY =
        tileAt(rect.x, newy).type == MAP_TILE_TYPE_WALL ||
        tileAt(rect.x + rect.width - 1, newy).type == MAP_TILE_TYPE_WALL ||
        tileAt(rect.x, newy + rect.height).type == MAP_TILE_TYPE_WALL ||
        tileAt(rect.x + rect.width - 1, newy + rect.height).type ==
            MAP_TILE_TYPE_WALL;
    if (!blockX)
      rect.x = newx;
    if (!blockY)
      rect.y = newy;
  }
};
