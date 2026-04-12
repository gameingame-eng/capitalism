#pragma once
#include "global.hpp"
#include "raylib.h"
#include "utils.hpp"

enum GameMenuVillageState { VILLAGE_STATE_SELECTING };

struct GameMenuVillageSelectionOption {
  const char *text;
  int hover = 0;
  GameMenuVillageSelectionOption(const char *text) : text(text) {}
};

class GameMenuVillage {
public:
  GameMenuVillageState state = VILLAGE_STATE_SELECTING;

  GameMenuVillageSelectionOption selections[7] = {
      GameMenuVillageSelectionOption("> Inn"),
      GameMenuVillageSelectionOption("> Notice Board"),
      GameMenuVillageSelectionOption("> Bank"),
      GameMenuVillageSelectionOption("> Blacksmith"),
      GameMenuVillageSelectionOption("> gas station :3"),
      GameMenuVillageSelectionOption("> Alchemist"),
      GameMenuVillageSelectionOption("> Armorsmith")};

  void draw() {
    switch (state) {
    case VILLAGE_STATE_SELECTING: {
      DrawText("Village", RENDER_W - MeasureText("Village", 64) - 20, 20, 64,
               WHITE);
      for (int i = 0; i < 8; i++) {
        GameMenuVillageSelectionOption opt = selections[i];
        DrawText(opt.text, 20 + 40 * ease_in_out_quadratic(opt.hover / 32.0f),
                 RENDER_W / 4 + i * 56, 48, WHITE);
      }

      break;
    }
    }
  }
  void update(float dt, Vector2 mouse_pos) {
    for (int i = 0; i < 8; i++) {
      GameMenuVillageSelectionOption &opt = selections[i];
      Rectangle hover_rect = {0, (float)RENDER_W / 4 + i * 56 - 56 / 4, 500,
                              56 * 1.5};
      if (mouse_pos.x > hover_rect.x &&
          mouse_pos.x < hover_rect.x + hover_rect.width &&
          mouse_pos.y > hover_rect.y &&
          mouse_pos.y < hover_rect.y + hover_rect.height) {
        opt.hover++;
        if (opt.hover > 32)
          opt.hover = 32;
      } else if (opt.hover > 0)
        opt.hover--;
    }
  }
};
