#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>
#include "../entities/building.h"
#include <memory>

constexpr int TOP_LEFT = 0;
constexpr int TOP_RIGHT = 1;
constexpr int BOTTOM_RIGHT = 2;
constexpr int BOTTOM_LEFT= 3;

constexpr int EMPTY = 0;
constexpr int USED = 1;

constexpr float BORDER_WIDTH = 1.0f;
constexpr float BOLD_BORDER_WIDTH = 4.0f;


class Tile {
  public:
    raylib::Vector2 pos;
    raylib::Color color;
    int status;
    bool isLight;
    std::unique_ptr<Building> building;

    Tile(int x, int y);
    void Render(const raylib::Camera2D& camera, bool hovered = false);
    void SetColor(raylib::Color nc);
    raylib::Vector2 Distance(raylib::Vector2, const raylib::Camera2D& camera);
    raylib::Vector2 GetScreenPos(const raylib::Camera2D& camera);
    raylib::Vector2 GetWorldPos();
    void DrawBorder(raylib::Color c, int border, float width, const raylib::Camera2D& camera);
    raylib::Color GetUIColor(bool hovered = false);
};
