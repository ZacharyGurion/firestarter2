#include "tile.h"
#include "city.h"
#include <vector>
#include <cmath>
#include <random>
#include "../entities/enemybuilding.h"

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
  Tile* best = nullptr;
  float bdist = 1e9f;
  for (int y = 0; y < grid.size(); y++) {
    for (int x = 0; x < grid[y].size(); x++) {
      Tile& t = grid[y][x];
      raylib::Vector2 distVec = t.Distance(mousePos);
      float dist = std::sqrt(distVec.x*distVec.x + distVec.y*distVec.y);
      if (dist < bdist) {
        bdist = dist;
        best = &t;
      }
    }
  }
  // DrawText(TextFormat("Position x:%i, y:%i", (int)mousePos.x, (int)mousePos.y), 200, 150, 20, WHITE);
  // DrawText(TextFormat("Distance: %i",(int)bdist), 200, 170, 20, WHITE);
  // raylib::Vector2 bestPos = best->GetScreenPos();
  // DrawLine(bestPos.x, bestPos.y, (int)mousePos.x, (int)mousePos.y, RED);
  return best;
}

void City::Render(raylib::Vector2 offset) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      grid[y][x].Render(offset);
    }
  }
}

void City::SpawnEnemyBuilding() {
  std::vector<std::pair<int, int>> emptyTiles;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      Tile& tile = grid[y][x];
      if (tile.status == EMPTY && !tile.building) {
        emptyTiles.push_back({x, y});
      }
    }
  }
  
  // If there are empty tiles, choose one randomly and spawn an enemy building
  if (!emptyTiles.empty()) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, emptyTiles.size() - 1);
    
    int randomIndex = distrib(gen);
    int x = emptyTiles[randomIndex].first;
    int y = emptyTiles[randomIndex].second;
    
    grid[y][x].building = std::make_unique<EnemyBuilding>();
  }
}
