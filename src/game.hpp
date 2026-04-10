#pragma once
#include "assets.hpp"
#include "raylib.h"
#include "utils.hpp"
#include <iterator>
#include <raymath.h>

#define PLAYER_CAMERA_FOLLOW_MARGIN 100

enum MapTile { MAP_TILE_GRASS, MAP_TILE_TREE, MAP_TILE_STONE };

class Game {
public:
  SceneTransitionFade from_main_menu_transition =
      SceneTransitionFade(16, DARKGREEN, ColorAlpha(DARKGREEN, 0));
  Camera2D gamecam;
  Rectangle player = {0, 0, MAP_TILE_SIZE, MAP_TILE_SIZE};
  float playerSpeed = 200;
  MapTile map[100][100] = {MAP_TILE_GRASS};
  int player_facing = 0;
  int player_anim_frame = 0;
  int player_anim_frame_real_max = 10;
  int player_anim_frame_real = 0;
  PlayerSpriteState player_state = PLAYER_SPRITE_STATE_WALK;

  Game() {
    gamecam.target = {player.x + player.width / 2,
                      player.y + player.height / 2};
    gamecam.offset = {RENDER_W / 2, RENDER_H / 2};
    gamecam.rotation = 0.0f;
    gamecam.zoom = 0.5f;

    for (int i = 0; i < 100; i++) {
      for (int j = 0; j < 100; j++) {
        if (Vector2Distance({(float)j + 0.5f, (float)i + 0.5f},
                            {50.0f, 50.0f}) > 5) {
          if (GetRandomValue(0, 6) == 3) {
            map[j][i] = MAP_TILE_TREE;
          }
        } else {
          map[j][i] = MAP_TILE_STONE;
        }
      }
    }
  }

  void update(float dt) {
    if (!from_main_menu_transition.finished) {
      from_main_menu_transition.update();
    }

    player_state = PLAYER_SPRITE_STATE_IDLE;

    if (IsKeyDown(KEY_A)) {
      player.x -= playerSpeed * dt;
      player_facing = 1;
      player_state = PLAYER_SPRITE_STATE_WALK;
    } else if (IsKeyDown(KEY_D)) {
      player.x += playerSpeed * dt;
      player_facing = 2;
      player_state = PLAYER_SPRITE_STATE_WALK;
    }
    if (IsKeyDown(KEY_W)) {
      player.y -= playerSpeed * dt;
      player_facing = 3;
      player_state = PLAYER_SPRITE_STATE_WALK;
    } else if (IsKeyDown(KEY_S)) {
      player.y += playerSpeed * dt;
      player_facing = 0;
      player_state = PLAYER_SPRITE_STATE_WALK;
    }

    if (std::abs((player.x + player.width / 2) - gamecam.target.x) > 100) {
      if (player.x > gamecam.target.x)
        gamecam.target.x =
            player.x + player.width / 2 - PLAYER_CAMERA_FOLLOW_MARGIN;
      else
        gamecam.target.x =
            player.x + player.width / 2 + PLAYER_CAMERA_FOLLOW_MARGIN;
    }
    if (std::abs((player.y + player.height / 2) - gamecam.target.y) > 75) {
      if (player.y > gamecam.target.y)
        gamecam.target.y =
            player.y + player.height / 2 - PLAYER_CAMERA_FOLLOW_MARGIN * 0.75;
      else
        gamecam.target.y =
            player.y + player.height / 2 + PLAYER_CAMERA_FOLLOW_MARGIN * 0.75;
    }

    player_anim_frame_real++;
    if (player_anim_frame_real == player_anim_frame_real_max) {
      player_anim_frame = (player_anim_frame + 1) % 2;
      player_anim_frame_real = 0;
    }
  }

  void draw() {
    BeginMode2D(gamecam);

    for (int i = 0; i < 100; i++) {
      for (int j = 0; j < 100; j++) {
        Rectangle source = {0, 0, 32, 32};
        Rectangle dest = {(i - 50) * MAP_TILE_SIZE, (j - 50) * MAP_TILE_SIZE,
                          MAP_TILE_SIZE, MAP_TILE_SIZE};
        Texture2D *tileset = &tileset_01;
        switch (map[j][i]) {
        case MAP_TILE_GRASS: {
          tileset = &tileset_02;
          break;
        }
        case MAP_TILE_TREE: {
          tileset = &tileset_b;
          source = {2 * 32, 2 * 32, 32, 32};
          break;
        }
        case MAP_TILE_STONE: {
          tileset = &tileset_02;
          source = {5 * 32, 3 * 32, 32, 32};
          break;
        }
        }
        tileset = &tileset_02;
        if (MAP_TILE_TREE == map[j][i]) {
          DrawTexturePro(*tileset, {0, 0, 32, 32}, dest, {0, 0}, 0, WHITE);
          tileset = &tileset_b;
        }

        DrawTexturePro(*tileset, source, dest, {0, 0}, 0, WHITE);
      }
    }

    DrawTexturePro(
        characters,
        GetPlayerSpriteSource(player_state, player_anim_frame, player_facing),
        player, {0, 0}, 0.0f, WHITE);

    EndMode2D();

    if (!from_main_menu_transition.finished) {
      DrawRectangle(0, 0, 800, 600, from_main_menu_transition.getColor());
    }
  }
};
