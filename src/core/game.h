#ifndef GAME_H
#define GAME_H

#include "../world/city.h"
#include <raylib.h>
#include <raylib-cpp.hpp>

class Game {
  private:
    raylib::Window window;
    City city;
    raylib::Vector2 cameraOffset;
    float arrowBounce;
    float bounceSpeed;

  public:
    Game(int width, int height);
    void Run();
    void Update();
    void Render();
    void HandleEvents();
};

#endif
