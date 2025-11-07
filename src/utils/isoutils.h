#ifndef ISOUTILS_H
#define ISOUTILS_H

#include <raylib.h>
#include <raylib-cpp.hpp>

namespace IsoUtils {
  constexpr int TILE_WIDTH = 80;
  constexpr int TILE_HEIGHT = TILE_WIDTH / 2;
  raylib::Vector2 GridToIso(int x, int y);
  raylib::Vector2 GridToIso(raylib::Vector2 vec);
  raylib::Vector2 IsoToGrid(float isoX, float isoY);
  raylib::Vector2 IsoToGrid(raylib::Vector2 vec);
  bool PointInDiamond(raylib::Vector2 mouse, raylib::Vector2 tilePos);
}

#endif
