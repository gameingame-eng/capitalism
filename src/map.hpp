#pragma once
#include "assets.hpp"
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
    return tmrfc(0, 0);
  case MAP_TILE_SOURCE_TREE:
    return tmrfc(2, 2);
  case MAP_TILE_SOURCE_STONE:
    return tmrfc(5, 0);
  }
}

struct MapTile {
  MapTileType type;
  MapTileSourceName name;
  Rectangle source;
  Texture2D *image;

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
