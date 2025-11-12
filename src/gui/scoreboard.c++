#include "scoreboard.h"
#include <raylib.h>
#include <raylib-cpp.hpp>
#include <string>

raylib::Font getFont() {
  return LoadFontEx("spleen.otf", FONT_SIZE, 0, 250);
}

Scoreboard::Scoreboard() 
  : score(0), position(SCOREBOARD_X_OFFSET, SCOREBOARD_Y_OFFSET) {
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
  DrawRectangleRounded(raylib::Rectangle{position.x, position.y, SCOREBOARD_WIDTH, SCOREBOARD_HEIGHT}, 0.3f, 8, DARKGRAY);
  
  std::string text = std::to_string(score);
  
  if (!font.IsValid()) {
    exit(0);
  }
  
  DrawTextEx(font, std::string("Score: "), {position.x + TEXT_OFFSET, position.y + TEXT_OFFSET}, FONT_SIZE, SPACING, WHITE);
  
  raylib::Vector2 pointsSize = MeasureTextEx(font, std::to_string(score).c_str(), FONT_SIZE, SPACING);

  DrawTextEx(font, std::to_string(score), {SCOREBOARD_WIDTH - TEXT_OFFSET - pointsSize.x, position.y + TEXT_OFFSET}, FONT_SIZE, SPACING, WHITE);
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
