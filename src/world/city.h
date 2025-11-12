#pragma once

#include "tile.h"
#include <vector>
#include <raylib-cpp.hpp>

class City {
  private:
    std::vector<std::vector<Tile>> grid;
    const int width;
    const int height;
  public:
    City(int w, int h);
    Tile* GetTileAtMouse(raylib::Vector2 mousePos, const raylib::Camera2D& camera);
    void Render(const raylib::Camera2D& camera);
    void SpawnEnemyBuilding();
};
