#include "tile.h"

#include "../utils/isoutils.h"
#include <raylib.h>
#include <raylib-cpp.hpp>

Tile::Tile(int x, int y) : pos(static_cast<float>(x), static_cast<float>(y)) {
  pos = raylib::Vector2(x, y);
  isLight = !((y) & 1);
  color = isLight ? GREEN : BLUE;
  status = EMPTY;
}

bool Tile::HandleClick() {
  if (status != USED) {
    status = USED;
    color = BLACK;
    return true;
  }
  return false;
}

void Tile::Render(raylib::Vector2 offset, bool hovered) {
  raylib::Vector2 screenPos = GetScreenPos();
  
  raylib::Vector2 top = raylib::Vector2(screenPos.x, screenPos.y - IsoUtils::TILE_HEIGHT / 2.0f);
  raylib::Vector2 right = raylib::Vector2(screenPos.x + IsoUtils::TILE_WIDTH / 2.0f, screenPos.y);
  raylib::Vector2 bottom = raylib::Vector2(screenPos.x, screenPos.y + IsoUtils::TILE_HEIGHT / 2.0f);
  raylib::Vector2 left = raylib::Vector2(screenPos.x - IsoUtils::TILE_WIDTH / 2.0f, screenPos.y);

  DrawTriangle(top, bottom, right, color);
  DrawTriangle(top, left, bottom, color);


  raylib::Color borderColor = BLACK;
  float borderWidth = 1.0f;
  if (hovered) {
    borderColor = RED;
    borderWidth = 4.0f;
    DrawLineEx(right, bottom, borderWidth, borderColor);
    DrawLineEx(bottom, left, borderWidth, borderColor);
  }
  DrawLineEx(top, right, borderWidth, borderColor);
  // DrawLineEx(right, bottom, borderWidth, borderColor);
  // DrawLineEx(bottom, left, borderWidth, borderColor);
  DrawLineEx(left, top, borderWidth, borderColor);

  // if (hovered) {
  //   DrawText(TextFormat("Tile %i, %i", (int)pos.x, (int)pos.y), 200, 100, 20, WHITE);
  // }
}

void Tile::DrawBorder(raylib::Color c, int border, float width) {
  raylib::Vector2 screenPos = GetScreenPos();
  raylib::Vector2 v1;
  raylib::Vector2 v2;
  switch (border) {
    case TOP_LEFT:
      v1 = raylib::Vector2(screenPos.x - IsoUtils::TILE_WIDTH / 2.0f, screenPos.y);
      v2 = raylib::Vector2(screenPos.x, screenPos.y - IsoUtils::TILE_HEIGHT / 2.0f);
      break;
    case TOP_RIGHT:
      v1 = raylib::Vector2(screenPos.x, screenPos.y - IsoUtils::TILE_HEIGHT / 2.0f);
      v2 = raylib::Vector2(screenPos.x + IsoUtils::TILE_WIDTH / 2.0f, screenPos.y);
      break;
    case BOTTOM_RIGHT:
      v1 = raylib::Vector2(screenPos.x + IsoUtils::TILE_WIDTH / 2.0f, screenPos.y);
      v2 = raylib::Vector2(screenPos.x, screenPos.y + IsoUtils::TILE_HEIGHT / 2.0f);
      break;
    case BOTTOM_LEFT:
      v2 = raylib::Vector2(screenPos.x, screenPos.y + IsoUtils::TILE_HEIGHT / 2.0f);
      v1 = raylib::Vector2(screenPos.x - IsoUtils::TILE_WIDTH / 2.0f, screenPos.y);
      break;
    default:
      return;
  }
  DrawLineEx(v1, v2, width, c);
}

void Tile::SetColor(raylib::Color nc) {
  color = nc;
}

raylib::Vector2 Tile::Distance(raylib::Vector2 point) {
  return GetScreenPos() - point;
}

raylib::Vector2 Tile::GetScreenPos() {
  float nx = pos.x*IsoUtils::TILE_WIDTH;
  float ny = pos.y*IsoUtils::TILE_HEIGHT*0.5f;
  if (((int)pos.y) & 1) {
    nx += IsoUtils::TILE_WIDTH*0.5f;
  }
  return raylib::Vector2(nx, ny);
}
