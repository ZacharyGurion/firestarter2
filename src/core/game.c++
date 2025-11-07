#include "game.h"
#include "../utils/isoutils.h"
#include "../world/tile.h"
#include "../world/city.h"
#include <raylib-cpp.hpp>
#include <cmath>

Game::Game(int width, int height)
  : window(width, height, "Fire Starter 2"),
    city(16, 16),
    arrowBounce(0.0f),
    bounceSpeed(3.0f) {
  cameraOffset.x = width / 2.0f;
  cameraOffset.y = height / 6.0f;
  SetTextureFilter(GetFontDefault().texture, TEXTURE_FILTER_TRILINEAR);
  HideCursor();
  SetTargetFPS(60);
}

void Game::Run() {
  while (!window.ShouldClose()) {
    Update();
    Render();
  }
}

void Game::Update() {
  arrowBounce += bounceSpeed * GetFrameTime();
  
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    raylib::Vector2 mousePos = GetMousePosition();
    Tile* clickedTile = city.GetTileAtScreen(mousePos, cameraOffset);
    if (clickedTile) {
      clickedTile->SetColor(BLACK);
    }
  }
}

void Game::Render() {
  BeginDrawing();
  ClearBackground(GRAY);
  city.Render(cameraOffset);
  
  raylib::Vector2 mousePos = GetMousePosition();
  Tile* hovered = city.GetTileAtMouse(mousePos, cameraOffset);
  
  if (hovered) {
    hovered->Render(cameraOffset, true);
    raylib::Vector2 tilePos = IsoUtils::GridToIso(hovered->pos) + cameraOffset;
    float bounceOffset = sin(arrowBounce) * 5.0f;
    float arrowY = tilePos.y - 5.0f + bounceOffset;
    raylib::Vector2 top = {tilePos.x, arrowY};
    raylib::Vector2 left = {tilePos.x - 10.0f, arrowY - 15.0f};
    raylib::Vector2 right = {tilePos.x + 10.0f, arrowY - 15.0f};
    raylib::Color outline = BLACK;
    DrawTriangle(top, right, left, RED);
    DrawLineV(left, right, outline);
    DrawLineV(right, top, outline);
    DrawLineV(top, left, outline);
  }
  
  EndDrawing();
}

void Game::HandleEvents() {
}
