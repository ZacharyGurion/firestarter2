#pragma once

#include "../world/city.h"
#include "../gui/scoreboard.h"
#include <raylib.h>
#include <raylib-cpp.hpp>

constexpr int GAME_WIDTH = 640;
constexpr int GAME_HEIGHT = 480;

constexpr float ARROW_WIDTH = 10.0f;
constexpr float ARROW_HEIGHT = 15.0f;
constexpr float ARROW_AMPLITUDE = 5.0f;
constexpr float ARROW_BOUNCE_SPEED = 3.0f;

constexpr float ENEMY_SPAWN_INTERVAL = 2.0f;

class Game {
  private:
    raylib::Window window;
    City city;
    Scoreboard scoreboard;
    raylib::Vector2 cameraOffset = { GAME_WIDTH / 2.0f, GAME_HEIGHT / 2.0f };
    float arrowBounce;
    float bounceSpeed;
    float spawnTimer;

  public:
    Game();
    void Run();
    void Update();
    void Render();
    void HandleEvents();
};
