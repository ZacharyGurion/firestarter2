#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>
#include <memory>
#include "../entities/building.h"
#include "../gui/scoreboard.h"

constexpr int TOP_LEFT = 0;
constexpr int TOP_RIGHT = 1;
constexpr int BOTTOM_RIGHT = 2;
constexpr int BOTTOM_LEFT= 3;

constexpr int COOLDOWN = 2;

constexpr float MIN_COOLDOWN = 2.0f;
constexpr float MAX_COOLDOWN = 8.0f;

constexpr float BORDER_WIDTH = 1.0f;
constexpr float BOLD_BORDER_WIDTH = 4.0f;

constexpr int SPREAD_CHANCE = 1000;
constexpr float DIAGONAL_PROBABILITY_MOD = 0.5f;

class City;

class Tile {
  private:
    City* parentCity;
  public:
    raylib::Vector2 pos;
    raylib::Color color;
    bool isLight;
    std::unique_ptr<Building> building;
    float cooldownTimer;

    std::array<Tile*, 4> orthoTiles;
    int orthoCount = 0;
    std::array<Tile*, 4> diagTiles;
    int diagCount = 0;
    std::array<Tile*, 4> adjTiles;
    int adjCount = 0;

    Tile(int x, int y, City* city);
    void Render(const raylib::Camera2D& camera, bool hovered = false);
    void Update();
    void HandleClick(Scoreboard& scoreboard_ptr);
    void SetColor(raylib::Color nc);

    raylib::Vector2 Distance(raylib::Vector2, const raylib::Camera2D& camera);
    raylib::Vector2 GetScreenPos(const raylib::Camera2D& camera);
    raylib::Vector2 GetWorldPos();
    void DrawBorder(raylib::Color c, int border, float width, const raylib::Camera2D& camera);
    raylib::Color GetUIColor(bool hovered = false);
};
