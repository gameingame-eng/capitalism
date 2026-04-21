#pragma once
#include "assets.hpp"
#include "enemy.hpp"
#include "gamestate.hpp"
#include "global.hpp"
#include "map.hpp"
#include "player.hpp"
#include "raylib.h"
#include "utils.hpp"
#include "raymath.h"
#include <algorithm>
#include <vector>

class Game {
public:
  SceneTransitionFade from_main_menu_transition =
      SceneTransitionFade(16, DARKGREEN, ColorAlpha(DARKGREEN, 0));
  Camera2D gamecam;
  MapTile map[100][100];
  std::vector<Enemy> enemies;
  GameState state;

  Player player;

  Game() {
    gamecam.target = {player.rect.x + player.rect.width / 2,
                      player.rect.y + player.rect.height / 2};
    gamecam.offset = {RENDER_W / 2, RENDER_H / 2};
    gamecam.rotation = 0.0f;
    gamecam.zoom = 0.5f;

    for (int y = 0; y < 100; y++) {
      for (int x = 0; x < 100; x++) {
        if (Vector2Distance({(float)x + 0.5f, (float)y + 0.5f},
                            {50.0f, 50.0f}) < 3) {
          map[y][x] = MapTile(MAP_TILE_SOURCE_STONE);
        } else {
          map[y][x] = MapTile(MAP_TILE_SOURCE_GRASS);
          if (GetRandomValue(0, 6) == 3) {
            map[y][x] = MapTile(MAP_TILE_SOURCE_TREE);
          }
          if (GetRandomValue(0, 20) == 5 &&
              map[y][x].type != MAP_TILE_TYPE_WALL) {
            enemies.push_back(
                Enemy({(x - 50) * MAP_TILE_SIZE, (y - 50) * MAP_TILE_SIZE,
                       MAP_TILE_SIZE, MAP_TILE_SIZE}));
          }
        }
      }
    }

    for (int y = 0; y < 100; y++) {
      for (int x = 0; x < 100; x++) {
        MapTileSourceName n = map[std::max(y - 1, 0)][x].name;
        MapTileSourceName e = map[y][std::min(x + 1, 99)].name;
        MapTileSourceName s = map[std::min(y + 1, 99)][x].name;
        MapTileSourceName w = map[y][std::max(x - 1, 0)].name;
        map[y][x].fix(n, e, s, w);
      }
    }
  }

  void update(float dt) {
    if (!from_main_menu_transition.finished) {
      from_main_menu_transition.update();
    }

    for (Enemy &e : enemies)
      e.update(dt,
               {player.hitbox.x + player.hitbox.width / 2,
                player.hitbox.y + player.hitbox.height / 2},
               map);
    player.update(map, dt, gamecam, state, enemies);
  }

  void draw() {
    BeginMode2D(gamecam);

    for (int i = 0; i < 100; i++) {
      for (int j = 0; j < 100; j++) {
        Rectangle source = {0, 0, 32, 32};
        Rectangle dest = {(i - 50) * MAP_TILE_SIZE, (j - 50) * MAP_TILE_SIZE,
                          MAP_TILE_SIZE, MAP_TILE_SIZE};
        if (map[j][i].name == MAP_TILE_SOURCE_TREE) {
          DrawTexturePro(tileset_02, {0, 0, 32, 32}, dest, {0, 0}, 0, WHITE);
        }
        DrawTexturePro(*map[j][i].image, map[j][i].source, dest, {0, 0}, 0,
                       WHITE);
      }
    }

    for (auto e : enemies)
      e.draw();

    player.draw();

    EndMode2D();

    DrawText(std::to_string(state.health).c_str(), 30, 30, 24, BLACK);
    if (!from_main_menu_transition.finished) {
      DrawRectangle(0, 0, 800, 600, from_main_menu_transition.getColor());
    }
  }
};
