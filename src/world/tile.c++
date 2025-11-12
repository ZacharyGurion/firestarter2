#include "tile.h"

#include "../utils/isoutils.h"
#include <raylib.h>
#include <raylib-cpp.hpp>
#include <random>
#include "../entities/enemybuilding.h"
#include "./city.h"
#include <emscripten/console.h>
#include <algorithm>
#include <format>
#include <string>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distrib(1, 10);
std::uniform_int_distribution<> enemyDistrib(0, 1);
std::uniform_real_distribution<> cooldown(MIN_COOLDOWN, MAX_COOLDOWN);
std::uniform_int_distribution<> spread(0, SPREAD_CHANCE);

Tile::Tile(int x, int y, City* city) : pos(static_cast<float>(x), static_cast<float>(y)) {
  pos = raylib::Vector2(x, y);
  isLight = !((y) & 1);
  color = isLight ? GREEN : BLUE;
  cooldownTimer = 0.0f;
  parentCity = city;

  if (distrib(gen) == 2) {
    if (enemyDistrib(gen) == 1) {
      building = std::make_unique<EnemyBuilding>();
    } else {
      building = std::make_unique<Building>();
    }
  }
}

void Tile::Render(const raylib::Camera2D& camera, bool hovered) {
  raylib::Vector2 worldPos = GetWorldPos();
  
  raylib::Vector2 top = raylib::Vector2(worldPos.x, worldPos.y - IsoUtils::TILE_HEIGHT * 0.5f);
  raylib::Vector2 right = raylib::Vector2(worldPos.x + IsoUtils::TILE_WIDTH * 0.5f, worldPos.y);
  raylib::Vector2 bottom = raylib::Vector2(worldPos.x, worldPos.y + IsoUtils::TILE_HEIGHT * 0.5f);
  raylib::Vector2 left = raylib::Vector2(worldPos.x - IsoUtils::TILE_WIDTH * 0.5f, worldPos.y);

  raylib::Color renderColor = (cooldownTimer < 0.001f) ? color : BLACK;
  DrawTriangle(top, bottom, right, renderColor);
  DrawTriangle(top, left, bottom, renderColor);


  raylib::Color borderColor = GetUIColor(hovered);
  if (hovered) {
    DrawLineEx(right, bottom, BOLD_BORDER_WIDTH, borderColor);
    DrawLineEx(bottom, left, BOLD_BORDER_WIDTH, borderColor);
    DrawLineEx(left, top, BOLD_BORDER_WIDTH, borderColor);
    DrawLineEx(top, right, BOLD_BORDER_WIDTH, borderColor);
    DrawText(TextFormat("Tile: %i, %i", (int)pos.x, (int)pos.y), 200, 50, 20, WHITE);
  } else {
    DrawLineEx(left, top, BORDER_WIDTH, borderColor);
    DrawLineEx(top, right, BORDER_WIDTH, borderColor);
  }

  if (building) {
    building->Render(worldPos, hovered);
  }
}

void Tile::Update() {
  if (dynamic_cast<EnemyBuilding*>(building.get())) {
    int tx = (int)pos.x;
    int ty = (int)pos.y;

    auto ortho = parentCity->GetOrtho(tx, ty);
    for (Tile* neighbor : ortho) {
      if (neighbor->building && !dynamic_cast<EnemyBuilding*>(neighbor->building.get())) {
        if (spread(gen) < 1) {
          neighbor->building = std::make_unique<EnemyBuilding>();
        emscripten_out(std::format("From tile (x: {}, y: {})", (int)pos.x, (int)pos.y).c_str());
        emscripten_out(std::format("Setting tile (x: {}, y: {}) to enemy", (int)neighbor->pos.x, (int)neighbor->pos.y).c_str());
          // emscripten_out(std::format("From tile (x: {}, y: {})", (int)pos.x, (int)pos.y).c_str());
          // emscripten_out(std::format("Setting tile (x: {}, y: {}) to enemy", (int)neighbor->pos.x, (int)neighbor->pos.y).c_str());
        }
      }
    }
    auto diag = parentCity->GetDiag(tx, ty);
    for (Tile* neighbor : diag) {
      if (neighbor->building && !dynamic_cast<EnemyBuilding*>(neighbor->building.get())) {
        if (spread(gen)*DIAGONAL_PROBABILITY_MOD < 1) {
          neighbor->building = std::make_unique<EnemyBuilding>();
        emscripten_out(std::format("From tile (x: {}, y: {})", (int)pos.x, (int)pos.y).c_str());
        emscripten_out(std::format("Setting tile (x: {}, y: {}) to enemy", (int)neighbor->pos.x, (int)neighbor->pos.y).c_str());
          // emscripten_out(std::format("From tile (x: {}, y: {})", (int)pos.x, (int)pos.y).c_str());
          // emscripten_out(std::format("Setting tile (x: {}, y: {}) to enemy", (int)neighbor->pos.x, (int)neighbor->pos.y).c_str());
        }
      }
    }
  }

  if (cooldownTimer >= 0.001f) {
    cooldownTimer -= GetFrameTime();
    if (cooldownTimer <= 0) {
      cooldownTimer = 0.0f;
      color = isLight ? GREEN : BLUE;
    }
  }
  if (cooldownTimer <= 0.001f && !building) {
    if (GetRandomValue(0, 99999) < 3) {
      building = std::make_unique<Building>();
    }
  }
}

void Tile::HandleClick(Scoreboard& scoreboard_ptr) {
  auto ortho = parentCity->GetOrtho((int)pos.x, (int)pos.y);
  for (Tile* neighbor : ortho) {
    emscripten_out(std::format("Clicked tile (x: {}, y: {})", (int)neighbor->pos.x, (int)neighbor->pos.y).c_str());
    emscripten_out(std::format("Building? {}", neighbor->building ? "true" : "false").c_str());
    if (neighbor->building && !dynamic_cast<EnemyBuilding*>(neighbor->building.get())) {
      if (spread(gen) < 30) {
        neighbor->building = std::make_unique<EnemyBuilding>();
        // emscripten_out(std::format("From tile (x: {}, y: {})", (int)pos.x, (int)pos.y).c_str());
        // emscripten_out(std::format("Setting tile (x: {}, y: {}) to enemy", (int)neighbor->pos.x, (int)neighbor->pos.y).c_str());
      }
    }
  }
  if (cooldownTimer <= 0.001f) {
    if (dynamic_cast<EnemyBuilding*>(building.get())) {
      cooldownTimer = cooldown(gen);
      building.reset();
      scoreboard_ptr.Increment();
    }
  }
}

raylib::Color Tile::GetUIColor(bool hovered) {
  if (hovered) {
    if (building) {
      return building.get()->color;
    } else {
      return WHITE;
    }
  }
  return BLACK;
}

void Tile::DrawBorder(raylib::Color c, int border, float width, const raylib::Camera2D& camera) {
  raylib::Vector2 worldPos = GetWorldPos();
  raylib::Vector2 v1;
  raylib::Vector2 v2;
  switch (border) {
    case TOP_LEFT:
      v1 = raylib::Vector2(worldPos.x - IsoUtils::TILE_WIDTH * 0.5f, worldPos.y);
      v2 = raylib::Vector2(worldPos.x, worldPos.y - IsoUtils::TILE_HEIGHT * 0.5f);
      break;
    case TOP_RIGHT:
      v1 = raylib::Vector2(worldPos.x, worldPos.y - IsoUtils::TILE_HEIGHT * 0.5f);
      v2 = raylib::Vector2(worldPos.x + IsoUtils::TILE_WIDTH * 0.5f, worldPos.y);
      break;
    case BOTTOM_RIGHT:
      v1 = raylib::Vector2(worldPos.x + IsoUtils::TILE_WIDTH * 0.5f, worldPos.y);
      v2 = raylib::Vector2(worldPos.x, worldPos.y + IsoUtils::TILE_HEIGHT * 0.5f);
      break;
    case BOTTOM_LEFT:
      v2 = raylib::Vector2(worldPos.x, worldPos.y + IsoUtils::TILE_HEIGHT * 0.5f);
      v1 = raylib::Vector2(worldPos.x - IsoUtils::TILE_WIDTH * 0.5f, worldPos.y);
      break;
    default:
      return;
  }
  DrawLineEx(v1, v2, width, c);
}

void Tile::SetColor(raylib::Color nc) {
  color = nc;
}

raylib::Vector2 Tile::Distance(raylib::Vector2 point, const raylib::Camera2D& camera) {
  return GetWorldPos() - GetScreenToWorld2D(point, camera);
}

raylib::Vector2 Tile::GetWorldPos() {
  float nx = pos.x*IsoUtils::TILE_WIDTH;
  float ny = pos.y*IsoUtils::TILE_HEIGHT*0.5f;
  if (((int)pos.y) & 1) {
    nx += IsoUtils::TILE_WIDTH*0.5f;
  }
  return raylib::Vector2(nx, ny);
}

raylib::Vector2 Tile::GetScreenPos(const raylib::Camera2D& camera) {
    return GetWorldToScreen2D(GetWorldPos(), camera);
}
