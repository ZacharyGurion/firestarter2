#include "tile.h"

#include "../utils/isoutils.h"
#include <raylib.h>
#include <raylib-cpp.hpp>
#include <random>
#include "../entities/enemybuilding.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distrib(1, 10);
std::uniform_int_distribution<> enemyDistrib(0, 1);

Tile::Tile(int x, int y) : pos(static_cast<float>(x), static_cast<float>(y)) {
  pos = raylib::Vector2(x, y);
  isLight = !((y) & 1);
  color = isLight ? GREEN : BLUE;
  status = EMPTY;

  // if (x < 3 && y < 4) {
  //   status = USED;
  //   color = ORANGE;
  //   return;
  // }
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

  DrawTriangle(top, bottom, right, color);
  DrawTriangle(top, left, bottom, color);


  raylib::Color borderColor = GetUIColor(hovered);
  if (hovered) {
    DrawLineEx(right, bottom, BOLD_BORDER_WIDTH, borderColor);
    DrawLineEx(bottom, left, BOLD_BORDER_WIDTH, borderColor);
    DrawLineEx(left, top, BOLD_BORDER_WIDTH, borderColor);
    DrawLineEx(top, right, BOLD_BORDER_WIDTH, borderColor);
  } else {
    DrawLineEx(left, top, BORDER_WIDTH, borderColor);
    DrawLineEx(top, right, BORDER_WIDTH, borderColor);
  }

  if (building) {
    building->Render(worldPos, hovered);
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
