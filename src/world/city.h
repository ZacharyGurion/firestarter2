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
    void Update();
    std::vector<std::vector<Tile>>& GetGrid() { return grid; }
    Tile* GetTile(int x, int y);
    std::vector<Tile*> GetOrtho(int x, int y);
    std::vector<Tile*> GetDiag(int x, int y);
    std::vector<Tile*> GetAdj(int x, int y);
};
