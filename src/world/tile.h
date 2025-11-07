#ifndef TILE_H
#define TILE_H

#include <raylib.h>
#include <raylib-cpp.hpp>

class Tile {
  public:
    raylib::Vector2 pos;
    raylib::Color color;
    bool isLight;

    Tile(int x, int y) : pos(static_cast<float>(x), static_cast<float>(y)) {
      pos = raylib::Vector2(x, y);
      isLight = !((x ^ y) & 1);
      color = isLight ? GREEN : BLUE;
    }
    void Render(raylib::Vector2 offset, bool hovered = false);
    void SetColor(raylib::Color nc);
    bool ContainsPoint(raylib::Vector2 point, raylib::Vector2 offset);
};
#endif
