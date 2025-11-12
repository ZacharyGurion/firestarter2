#pragma once

#include <raylib-cpp.hpp>

constexpr float FONT_SIZE = 24.0f;
constexpr float SPACING = 2.0f;
constexpr float SCOREBOARD_X_OFFSET = 5.0f;
constexpr float SCOREBOARD_Y_OFFSET = 5.0f;
constexpr float SCOREBOARD_WIDTH = 150.0f;
constexpr float SCOREBOARD_HEIGHT = 36.0f;
constexpr float TEXT_OFFSET = (SCOREBOARD_HEIGHT - FONT_SIZE)/2.0f;

class Scoreboard {
  private:
    int score;
    raylib::Vector2 position;
    raylib::Font font;
  public:
    Scoreboard();
    Scoreboard(float x, float y);
    ~Scoreboard();
    void Render();
    void Increment();
    int GetScore() const;
    void Reset();
};
