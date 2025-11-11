#include "game.h"
#include "../world/tile.h"
#include "../world/city.h"
#include "../entities/enemybuilding.h"
#include <raylib-cpp.hpp>
#include <cmath>

Game::Game(int width, int height)
  : window(width, height, "Fire Starter 2"),
    city(11, 31),
    scoreboard(10.0f, 10.0f),
    arrowBounce(0.0f),
    bounceSpeed(3.0f) {
  cameraOffset.x = width / 2.0f;
  cameraOffset.y = height / 6.0f;
  // SetTextureFilter(GetFontDefault().texture, TEXTURE_FILTER_ANISOTROPIC_16X);
  // SetConfigFlags(FLAG_MSAA_4X_HINT);
  // HideCursor();
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
    Tile* clickedTile = city.GetTileAtMouse(mousePos, cameraOffset);
    if (clickedTile && clickedTile->status != USED) {
      if (dynamic_cast<EnemyBuilding*>(clickedTile->building.get())) {
        clickedTile->status = USED;
        clickedTile->color = BLACK;
        clickedTile->building.reset();
        scoreboard.Increment();
      }
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
    float bounceOffset = sin(arrowBounce) * 5.0f;
    
    // Calculate arrow position based on hovered tile
    raylib::Vector2 tilePos = hovered->GetScreenPos();
    float arrowY = tilePos.y - 5.0f + bounceOffset;
    raylib::Vector2 top = {tilePos.x, arrowY};
    raylib::Vector2 left = {tilePos.x - 10.0f, arrowY - 15.0f};
    raylib::Vector2 right = {tilePos.x + 10.0f, arrowY - 15.0f};
    raylib::Color outline = BLACK;
    raylib::Color fillColor = hovered->GetUIColor(true);
    DrawTriangle(top, right, left, fillColor);
    DrawLineV(left, right, outline);
    DrawLineV(right, top, outline);
    DrawLineV(top, left, outline);
  }
  
  scoreboard.Render();
  
  EndDrawing();
}

void Game::HandleEvents() {
}
