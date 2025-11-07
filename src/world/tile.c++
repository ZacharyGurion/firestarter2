#include "tile.h"

#include "../utils/isoutils.h"
#include <raylib.h>
#include <raylib-cpp.hpp>

void Tile::Render(raylib::Vector2 offset, bool hovered) {
  raylib::Vector2 isoPos = IsoUtils::GridToIso(pos);
  isoPos.x += offset.x;
  isoPos.y += offset.y;

  raylib::Vector2 right = {isoPos.x + IsoUtils::TILE_WIDTH / 2.0f, isoPos.y + IsoUtils::TILE_HEIGHT / 2.0f};
  raylib::Vector2 bottom = {isoPos.x, isoPos.y + IsoUtils::TILE_HEIGHT};
  raylib::Vector2 left = {isoPos.x - IsoUtils::TILE_WIDTH / 2.0f, isoPos.y + IsoUtils::TILE_HEIGHT / 2.0f};

  //isoPos is top of tile drawing
  DrawTriangle(isoPos, bottom, right, color);
  DrawTriangle(isoPos, left, bottom, color);

  raylib::Color borderColor = hovered ? RED : BLACK;
  float borderWidth = hovered ? 3.0f : 1.0f;
  DrawLineEx(isoPos, right, borderWidth, borderColor);
  DrawLineEx(right, bottom, borderWidth, borderColor);
  DrawLineEx(bottom, left, borderWidth, borderColor);
  DrawLineEx(left, isoPos, borderWidth, borderColor);
}

bool Tile::ContainsPoint(raylib::Vector2 point, raylib::Vector2 offset) {
  return false;
}

void Tile::SetColor(raylib::Color nc) {
  color = nc;
}
