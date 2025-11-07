#ifndef CITY_H
#define CITY_H

#include "tile.h"
#include <vector>

class City {
  private:
    std::vector<std::vector<Tile>> grid;
    const int width;
    const int height;
  public:
    City(int w, int h);
    Tile* GetTileAtScreen(raylib::Vector2 screenPos, raylib::Vector2 offset);
    Tile* GetTileAtMouse(raylib::Vector2 mousePos, raylib::Vector2 offset);
    void Render(raylib::Vector2 offset);
};
#endif
