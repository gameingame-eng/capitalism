#pragma once
#include "assets.hpp"
#include <iostream>
#include <raylib.h>

enum MapTileType { MAP_TILE_TYPE_WALL, MAP_TILE_TYPE_FLOOR };

enum MapTileSourceName {
  MAP_TILE_SOURCE_TREE,
  MAP_TILE_SOURCE_GRASS,
  MAP_TILE_SOURCE_STONE
};

inline Rectangle tmrfc(int x, int y) {
  return {(float)x * 32, (float)y * 32, 32, 32};
}

inline Rectangle MapTileSourceNameToSource(MapTileSourceName s) {
  switch (s) {
  case MAP_TILE_SOURCE_GRASS:
    return tmrfc(1, 9);
  case MAP_TILE_SOURCE_TREE:
    return tmrfc(2, 2);
  case MAP_TILE_SOURCE_STONE:
    return tmrfc(5, 0);
  }
  return tmrfc(0, 0); // never happens, just to satisfy compiler
}

struct MapTile {
  MapTileType type;
  MapTileSourceName name;
  Rectangle source;
  Texture2D *image;
  bool fixed = false; // check if it has been correctly permuted

  void fix(MapTileSourceName north, MapTileSourceName east,
           MapTileSourceName south, MapTileSourceName west) {
    if (fixed)
      return;

    switch (this->name) {
    case MAP_TILE_SOURCE_STONE:
      if (north == MAP_TILE_SOURCE_GRASS || north == MAP_TILE_SOURCE_TREE) {
        if (west == MAP_TILE_SOURCE_GRASS || west == MAP_TILE_SOURCE_TREE) {
          this->source = tmrfc(4, 1);
        }
        if (east == MAP_TILE_SOURCE_GRASS || east == MAP_TILE_SOURCE_TREE) {
          this->source = tmrfc(5, 1);
        }
      }
      if (south == MAP_TILE_SOURCE_GRASS || south == MAP_TILE_SOURCE_TREE) {
        if (west == MAP_TILE_SOURCE_GRASS || west == MAP_TILE_SOURCE_TREE) {
          this->source = tmrfc(4, 2);
        }
        if (east == MAP_TILE_SOURCE_GRASS || east == MAP_TILE_SOURCE_TREE) {
          this->source = tmrfc(5, 2);
        }
      }
      break;
    }

    fixed = true;
  }

  MapTile(MapTileSourceName s) {
    this->name = s;
    this->source = MapTileSourceNameToSource(s);
    this->image = &tileset_02;
    switch (s) {
    case MAP_TILE_SOURCE_GRASS:
      this->type = MAP_TILE_TYPE_FLOOR;
      this->image = &tileset_02;
      break;
    case MAP_TILE_SOURCE_STONE:
      this->type = MAP_TILE_TYPE_FLOOR;
      this->image = &tileset_02;
      break;
    case MAP_TILE_SOURCE_TREE:
      this->type = MAP_TILE_TYPE_WALL;
      this->image = &tileset_b;
      break;
    }
  }

  MapTile() {
    type = MAP_TILE_TYPE_FLOOR;
    name = MAP_TILE_SOURCE_GRASS;
    source = {0, 0, 1, 1};
    image = &tileset_02;
  }
};
