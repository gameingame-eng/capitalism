#ifndef H_GLOBALS
#define H_GLOBALS

#define RENDER_W 800.0f
#define RENDER_H 600.0f

#define MAP_TILE_SIZE 100.0f

#define PLAYER_CAMERA_FOLLOW_MARGIN 100

inline bool exitGame = false;

enum GameMenuState {
  GMSTATE_MAIN,
  GMSTATE_GAME,
  GMSTATE_VILLAGE,
  GMSTATE_DEATH
};

#endif
