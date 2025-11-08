#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <raylib-cpp.hpp>

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

#endif
