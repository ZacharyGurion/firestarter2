#include "tile.h"
#include "city.h"
#include "../utils/isoutils.h"
#include <vector>
#include <cmath>

City::City(int w, int h) : width(w), height(h) {
  grid.resize(height);
  for (int y = 0; y < height; y++) {
    grid[y].reserve(width);
    for (int x = 0; x < width; x++) {
      grid[y].emplace_back(x, y);
    }
  }
}

Tile* City::GetTileAtMouse(raylib::Vector2 mousePos, raylib::Vector2 offset) {
  raylib::Vector2 adj = mousePos - offset;
  raylib::Vector2 gridF = IsoUtils::IsoToGrid(adj.x, adj.y);
  int cx = (int) std::floor(gridF.x);
  int cy = (int) std::floor(gridF.y);

  Tile* best = nullptr;
  float minDist = 1e9f;

  for (int y = cy; y <= cy+1; y++) {
    if (y < 0 || y >= height) continue;
    for (int x = cx; x <= cx+1; x++) {
      if (x < 0 || x >= width) continue;
      Tile& t = grid[y][x];
      raylib::Vector2 tilePos = IsoUtils::GridToIso(x, y);
      if (IsoUtils::PointInDiamond(adj, tilePos)) {
        float dx = adj.x - tilePos.x;
        float dy = adj.y - tilePos.y;
        float dist = dx*dx + dy*dy;
        if (dist < minDist) {
          minDist = dist;
          best = &t;
        }
      }
    }
  }
  return best;
}

void City::Render(raylib::Vector2 offset) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      grid[y][x].Render(offset);
    }
  }
}

Tile* City::GetTileAtScreen(raylib::Vector2 screenPos, raylib::Vector2 offset) {
  //raylib::Vector2 adjPos = {screenPos.x - offset.x, screenPos.y - offset.y};
  raylib::Vector2 adjPos  = screenPos - offset;
  raylib::Vector2 cartPos = IsoUtils::IsoToGrid(adjPos);
  
  int gridX = (int)round(cartPos.x);
  int gridY = (int)round(cartPos.y);

  if (gridX >= 0 && gridX < width && gridY >= 0 && gridY < height) {
    return &grid[gridY][gridX];
  }
  return nullptr;
}
