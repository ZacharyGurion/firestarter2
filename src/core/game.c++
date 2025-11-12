#include "game.h"
#include "../world/tile.h"
#include "../world/city.h"
#include "../entities/enemybuilding.h"
#include "../utils/isoutils.h"
#include <raylib-cpp.hpp>
#include <raymath.h>
#include <cmath>

Game::Game()
  : window(GAME_WIDTH, GAME_HEIGHT, "Fire Starter 2"),
    city(11, 31),
    scoreboard(10.0f, 10.0f),
    arrowBounce(0.0f),
    bounceSpeed(3.0f) {
      camera.offset = { GAME_WIDTH * 0.5f, GAME_HEIGHT * 0.5f };
      camera.target = { 352.0f, 240.0f };
      camera.rotation = 0.0f;
      camera.zoom = 1.0f;
  // camera is now a constexpr member, initialized in game.h
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
  float dt = GetFrameTime();
  arrowBounce += bounceSpeed * dt;

  city.Update();
  
  // Per-frame processing
  // auto& grid = city.GetGrid();
  // for (auto& row : grid) {
  //   for (auto& tile : row) {
  //     // Handle cooldown
  //     if (tile.status == COOLDOWN) {
  //       tile.cooldownTimer -= dt;
  //       if (tile.cooldownTimer <= 0) {
  //         tile.status = EMPTY;
  //         tile.color = tile.isLight ? GREEN : BLUE;
  //       }
  //     }
  //     if (tile.status == EMPTY && !tile.building) {
  //       if (GetRandomValue(0, 99999) < 10) {
  //         pendingSpawns.push_back(&tile);
  //       }
  //     }
  //   }
  // }

  // if (!pendingSpawns.empty()) {
  //   Tile* tileToSpawn = pendingSpawns.back();
  //   pendingSpawns.pop_back();
  //   if (tileToSpawn->status == EMPTY && !tileToSpawn->building) {
  //       tileToSpawn->building = std::make_unique<Building>(); // Spawn normal building
  //   }
  // }

  raylib::Vector2 cameraMovement(0.0f, 0.0f);
  if (IsKeyDown(KEY_W)) cameraMovement.y -= 1.0f;
  if (IsKeyDown(KEY_S)) cameraMovement.y += 1.0f;
  if (IsKeyDown(KEY_A)) cameraMovement.x -= 1.0f;
  if (IsKeyDown(KEY_D)) cameraMovement.x += 1.0f;

  if (cameraMovement.LengthSqr() > 0.0f) {
      cameraMovement = cameraMovement.Normalize();
      camera.target = (raylib::Vector2)Vector2Add(camera.target, Vector2Scale(cameraMovement, CAMERA_MOVE_SPEED * dt));
  }
  
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    raylib::Vector2 mousePos = GetMousePosition();
    Tile* clickedTile = city.GetTileAtMouse(mousePos, camera);
    clickedTile->HandleClick(scoreboard);
    // if (clickedTile && clickedTile->cooldownTimer <= 0.001f) {
    //   if (dynamic_cast<EnemyBuilding*>(clickedTile->building.get())) {
    //     clickedTile->cooldownTimer = GetRandomValue(MIN_COOLDOWN*10, MAX_COOLDOWN*10)/10.0f;
    //     clickedTile->building.reset();
    //     scoreboard.Increment();
    //   }
    // }
  }
}

void Game::Render() {
  BeginDrawing();
  ClearBackground(GRAY);

  raylib::Vector2 mousePos = GetMousePosition();
  Tile* hovered = city.GetTileAtMouse(mousePos, camera);

  camera.BeginMode();

  city.Render(camera);
  
  if (hovered) {
    hovered->Render(camera, true);
  }

  camera.EndMode();

  if (hovered) {
    float bounceOffset = sin(arrowBounce) * ARROW_AMPLITUDE;
    
    raylib::Vector2 tilePos = hovered->GetScreenPos(camera);
    float arrowY = tilePos.y - 2.0f*ARROW_AMPLITUDE + bounceOffset;
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
