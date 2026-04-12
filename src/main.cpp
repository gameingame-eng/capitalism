#include "game.hpp"
#include "global.hpp"
#include "menu_main.hpp"
#include "village.hpp"
#include <cmath>
#include <raylib.h>

Texture2D tileset_01;
Texture2D tileset_02;
Texture2D tileset_04;
Texture2D tileset_05;
Texture2D tileset_b;
Texture2D characters;

int main() {
  InitWindow(RENDER_W, RENDER_H, "Capitalism");
  LoadAssets();
  // SetWindowState(FLAG_FULLSCREEN_MODE);
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  enum GameMenuState game_menu_state = GMSTATE_GAME;

  RenderTexture2D render_texture = LoadRenderTexture(RENDER_W, RENDER_H);
  GameMenuMain main_menu;
  GameMenuVillage village_menu;
  Game game;

  SetTargetFPS(32);

  while (!WindowShouldClose() && !exitGame) {
    DrawFPS(0, 0);
    // for render texture
    int rw = GetRenderWidth();
    int rh = GetRenderHeight();
    float scale = fminf((float)rw / RENDER_W, (float)rh / RENDER_H);
    int fw = (int)(RENDER_W * scale);
    int fh = (int)(RENDER_H * scale);
    int ox = (rw - fw) / 2;
    int oy = (rh - fh) / 2;
    //-----
    Vector2 mouse_pos = GetMousePosition();
    mouse_pos.x = (mouse_pos.x - ox) / scale;
    mouse_pos.y = (mouse_pos.y - oy) / scale;

    float dt = GetFrameTime();

    switch (game_menu_state) {
    case GMSTATE_MAIN: {
      main_menu.update(mouse_pos, game_menu_state);
      break;
    }
    case GMSTATE_GAME: {
      game.update(dt);
      break;
    }
    case GMSTATE_VILLAGE: {
      village_menu.update(dt, mouse_pos);
    }
    };

    ClearBackground(WHITE);
    BeginTextureMode(render_texture);
    DrawRectangle(0, 0, 800, 600, BLACK);

    if (game_menu_state == GMSTATE_MAIN) {
      main_menu.draw(mouse_pos);
    } else if (game_menu_state == GMSTATE_GAME) {
      game.draw();
    } else if (game_menu_state == GMSTATE_VILLAGE) {
      village_menu.draw();
    }

    EndTextureMode();

    BeginDrawing();

    // draw render texture

    DrawTexturePro(render_texture.texture, {0, 0, RENDER_W, -RENDER_H},
                   {(float)ox, (float)oy, (float)fw, (float)fh}, {0, 0}, 0,
                   WHITE);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
