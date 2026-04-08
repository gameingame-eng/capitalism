#include "utils.hpp"
#include <cmath>
#include <raylib.h>

#define RENDER_W 800.0f
#define RENDER_H 600.0f

bool exitGame = false;

class SceneTransition {
public:
  int frames = 0;
  int maxFrames = 0;
  Color start, end;
  bool finished = false;

  void update() {
    if (finished)
      return;
    frames++;
    if (maxFrames == frames) {
      finished = true;
    }
  }

  SceneTransition(int maxFrames, Color start, Color end)
      : maxFrames(maxFrames), start(start), end(end) {};

  Color getColor() {
    return ColorLerp(start, end, ease_in_out_cubic(frames / maxFrames));
  }
};

enum GameMenuState { GMSTATE_MAIN, GMSTATE_GAME, GMSTATE_SHOP, GMSTATE_DEATH };

class GameMenuMain {
public:
  const Texture2D gh_logo = LoadTexture("assets/github.jpg");
  bool is_switching_to_game = false;
  SceneTransition transition =
      SceneTransition(64, ColorAlpha(DARKGREEN, 0), DARKGREEN);
  float play_text_hovered_time = 0;
  float quit_text_hovered_time = 0;
  bool play_text_hovered, quit_text_hovered;
  const Rectangle play_text_box = {RENDER_W / 2 - 100, RENDER_H / 3 + 64 + 8,
                                   200, 48 + 16};
  const Rectangle quit_text_box = {
      RENDER_W / 2 - 100, RENDER_H / 3 + 64 + 16 + 48 + 8, 200, 48 + 12};

  void draw(Vector2 mouse_pos) {
    DrawTexture(gh_logo, 0, 0, WHITE);
    DrawTextCentered("game title", RENDER_W / 2, RENDER_H / 3, 64, GREEN);
    play_text_hovered = mouse_pos.x > play_text_box.x &&
                        mouse_pos.x < play_text_box.x + play_text_box.width &&
                        mouse_pos.y > play_text_box.y &&
                        mouse_pos.y < play_text_box.y + play_text_box.height;
    if (play_text_hovered && play_text_hovered_time < 30)
      play_text_hovered_time++;
    else if (!play_text_hovered && play_text_hovered_time > 0)
      play_text_hovered_time--;
    quit_text_hovered = mouse_pos.x > quit_text_box.x &&
                        mouse_pos.x < quit_text_box.x + quit_text_box.width &&
                        mouse_pos.y > quit_text_box.y &&
                        mouse_pos.y < quit_text_box.y + quit_text_box.height;
    if (quit_text_hovered && quit_text_hovered_time < 30)
      quit_text_hovered_time++;
    else if (!quit_text_hovered && quit_text_hovered_time > 0)
      quit_text_hovered_time--;
    float play_text_x_offset =
        12 * ease_in_out_cubic(play_text_hovered_time / 30.0f);
    float quit_text_x_offset =
        12 * ease_in_out_cubic(quit_text_hovered_time / 30.0f);
    DrawTextCentered("> play", RENDER_W / 2 + play_text_x_offset,
                     RENDER_H / 3 + 64 + 16, 48, WHITE);
    DrawTextCentered("> quit", RENDER_W / 2 + quit_text_x_offset,
                     RENDER_H / 3 + 64 + 16 + 48 + 8, 48, WHITE);
    if (is_switching_to_game) {
      DrawRectangle(0, 0, RENDER_W, RENDER_H, transition.getColor());
    }
  }

  void update(Vector2 mouse_pos, GameMenuState &state) {
    if (is_switching_to_game) {
      transition.update();
      if (transition.finished) {
        state = GMSTATE_GAME;
      }
      return;
    }
    if (quit_text_hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      exitGame = true;
    }
    if (play_text_hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      is_switching_to_game = true;
    }
  }
};

int main() {
  InitWindow(RENDER_W, RENDER_H, "Capitalism");
  SetWindowState(FLAG_FULLSCREEN_MODE);

  enum GameMenuState game_menu_state = GMSTATE_MAIN;

  RenderTexture2D render_texture = LoadRenderTexture(RENDER_W, RENDER_H);
  GameMenuMain main_menu;

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

    main_menu.update(mouse_pos, game_menu_state);

    ClearBackground(WHITE);
    BeginTextureMode(render_texture);
    DrawRectangle(0, 0, 800, 600, BLACK);

    if (game_menu_state == GMSTATE_MAIN) {
      main_menu.draw(mouse_pos);
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
