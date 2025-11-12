#include "game.h"
#include "../world/tile.h"
#include "../world/city.h"
#include "../entities/enemybuilding.h"
#include <raylib-cpp.hpp>
#include <cmath>

Game::Game()
  : window(GAME_WIDTH, GAME_HEIGHT, "Fire Starter 2"),
    city(11, 31),
    scoreboard(10.0f, 10.0f),
    arrowBounce(0.0f),
    bounceSpeed(3.0f),
    spawnTimer(0.0f) {
  // cameraOffset is now a constexpr member, initialized in game.h
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
  
  spawnTimer += GetFrameTime();
  if (spawnTimer >= ENEMY_SPAWN_INTERVAL) {
    city.SpawnEnemyBuilding();
    spawnTimer = 0.0f;
  }
  
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
    float bounceOffset = sin(arrowBounce) * ARROW_AMPLITUDE;
    
    raylib::Vector2 tilePos = hovered->GetScreenPos();
    float arrowY = tilePos.y - ARROW_AMPLITUDE + bounceOffset;
    raylib::Vector2 top = {tilePos.x, arrowY};
    raylib::Vector2 left = {tilePos.x - ARROW_WIDTH, arrowY - ARROW_HEIGHT};
    raylib::Vector2 right = {tilePos.x + ARROW_WIDTH, arrowY - ARROW_HEIGHT};
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
