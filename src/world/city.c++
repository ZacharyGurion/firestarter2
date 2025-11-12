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
      grid[y].emplace_back(x, y, this);
    }
  }
}

Tile* City::GetTileAtMouse(raylib::Vector2 mousePos, const raylib::Camera2D& camera) {
  raylib::Vector2 worldMousePos = GetScreenToWorld2D(mousePos, camera);
  Tile* best = nullptr;
  float bdist = 1e9f;
  for (int y = 0; y < grid.size(); y++) {
    for (int x = 0; x < grid[y].size(); x++) {
      Tile& t = grid[y][x];
      raylib::Vector2 distVec = t.GetWorldPos() - worldMousePos;
      float dist = std::sqrt(distVec.x*distVec.x + distVec.y*distVec.y);
      if (dist < bdist) {
        bdist = dist;
        best = &t;
      }
    }
  }
  return best;
}

void City::Render(const raylib::Camera2D& camera) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      grid[y][x].Render(camera);
    }
  }
}

void City::Update() {
  for (auto& row : grid) {
    for (auto& tile : row) {
      tile.Update();
    }
  }
}

Tile* City::GetTile(int x, int y) {
  if (y < 0 || y >= height) {
    return nullptr;
  }
  if (x < 0 || x >= width) {
    return nullptr;
  }
  return &grid[y][x];
}

std::vector<Tile*> City::GetDiag(int x, int y) {
  std::vector<Tile*> neighbors;
  int offsets[4][2] = {{0, -2}, {1, 0}, {0, 2}, {-1, 0}};

  for (const auto& offset : offsets) {
    int nx = x + offset[0];
    int ny = y + offset[1];
    
    Tile* neighbor = GetTile(nx, ny);
    if (neighbor) {
      neighbors.push_back(neighbor);
    }
  }
  return neighbors;
}

std::vector<Tile*> City::GetOrtho(int x, int y) {
  std::vector<Tile*> neighbors;
  int offsets[4][2] = {{-(!(y & 1)), -1}, {(y & 1), -1}, {(y & 1), 1}, {-(!(y & 1)), 1}};

  for (const auto& offset : offsets) {
    int nx = x + offset[0];
    int ny = y + offset[1];
    
    Tile* neighbor = GetTile(nx, ny);
    if (neighbor) {
      neighbors.push_back(neighbor);
    }
  }
  return neighbors;
}

std::vector<Tile*> City::GetAdj(int x, int y) {
  std::vector<Tile*> orth = GetOrtho(x, y);
  std::vector<Tile*> diag = GetDiag(x, y);
  
  orth.insert(orth.end(), diag.begin(), diag.end());
  return orth;
}
