#include "scoreboard.h"
#include <raylib.h>
#include <raylib-cpp.hpp>
#include <string>

float fontSize = 32.0f;
float spacing = 2.0f;

raylib::Font getFont() {
  return LoadFontEx("spleen.otf", fontSize, 0, 250);
}

Scoreboard::Scoreboard() 
  : score(0), position(10.0f, 10.0f) {
    font = getFont();
}

Scoreboard::Scoreboard(float x, float y) 
  : score(0), position(x, y) {
    font = getFont();
}

Scoreboard::~Scoreboard() {
  if (font.IsValid()) {
    font.Unload();
  }
}

void Scoreboard::Render() {
  DrawRectangleRounded(raylib::Rectangle{position.x - 5, position.y - 5, 180, 40}, 0.3f, 8, DARKGRAY);
  
  std::string text = std::to_string(score);
  
  if (!font.IsValid()) {
    exit(0);
  }
  
  DrawTextEx(font, std::string("Score: "), {position.x, position.y}, fontSize, spacing, WHITE);
  
  raylib::Vector2 pointsSize = MeasureTextEx(font, std::to_string(score).c_str(), fontSize, spacing);

  DrawTextEx(font, std::to_string(score), {180 - 5 - pointsSize.x, position.y}, fontSize, spacing, WHITE);
}

void Scoreboard::Increment() {
  score++;
}

int Scoreboard::GetScore() const {
  return score;
}

void Scoreboard::Reset() {
  score = 0;
}
