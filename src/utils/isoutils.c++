#include "isoutils.h"
#include <cmath>
#include <raylib.h>
#include <raylib-cpp.hpp>

namespace IsoUtils {
  raylib::Vector2 GridToIso(int x, int y) {
    float isoX = (x - y) * (TILE_WIDTH / 2.0f);
    float isoY = (x + y) * (TILE_HEIGHT / 2.0f);
    return raylib::Vector2(isoX, isoY);
  }

  raylib::Vector2 GridToIso(raylib::Vector2 vec) {
    float isoX = (vec.x - vec.y) * (TILE_WIDTH / 2.0f);
    float isoY = (vec.x + vec.y) * (TILE_HEIGHT / 2.0f);
    return raylib::Vector2(isoX, isoY);
  }

  raylib::Vector2 IsoToGrid(float isoX, float isoY) {
    float gridX = (isoX / (TILE_WIDTH / 2.0f) + isoY / (TILE_HEIGHT / 2.0f)) / 2.0f;
    float gridY = (isoY / (TILE_HEIGHT / 2.0f) - isoX / (TILE_WIDTH / 2.0f)) / 2.0f;
    return raylib::Vector2(gridX, gridY);
  }

  raylib::Vector2 IsoToGrid(raylib::Vector2 vec) {
    float gridX = (vec.x / (TILE_WIDTH / 2.0f) + vec.y / (TILE_HEIGHT / 2.0f)) / 2.0f;
    float gridY = (vec.y / (TILE_HEIGHT / 2.0f) - vec.x / (TILE_WIDTH / 2.0f)) / 2.0f;
    return raylib::Vector2(gridX, gridY);
  }

  bool PointInDiamond(raylib::Vector2 mouse, raylib::Vector2 tilePos) {
    float dx = mouse.x - tilePos.x;
    float dy = mouse.y - tilePos.y;
    float w = IsoUtils::TILE_WIDTH / 2.0f;
    float h = IsoUtils::TILE_HEIGHT / 2.0f;

    return (std::abs(dx/w) + std::abs(dy/h)) <= 1.0f;
  }
};
