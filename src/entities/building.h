#ifndef BUILDING_H
#define BUILDING_H

#include <raylib.h>
#include <raylib-cpp.hpp>

class Building {
  public:
    Building();
    virtual ~Building() = default;
    Color color = YELLOW;
    virtual void Render(raylib::Vector2 pos, bool hovered = false);
};
#endif
