#ifndef TILE_H
#define TILE_H

#include <raylib.h>
#include <raylib-cpp.hpp>

constexpr int TOP_LEFT = 0;
constexpr int TOP_RIGHT = 1;
constexpr int BOTTOM_RIGHT = 2;
constexpr int BOTTOM_LEFT= 3;

constexpr int EMPTY = 0;
constexpr int USED = 1;

class Tile {
  public:
    raylib::Vector2 pos;
    raylib::Color color;
    int status;
    bool isLight;

    Tile(int x, int y);
    void Render(raylib::Vector2 offset, bool hovered = false);
    void SetColor(raylib::Color nc);
    bool HandleClick();
    raylib::Vector2 Distance(raylib::Vector2);
    raylib::Vector2 GetScreenPos();
    void DrawBorder(raylib::Color c, int border, float width);
};
#endif
