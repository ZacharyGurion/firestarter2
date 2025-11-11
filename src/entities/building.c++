#include "building.h"
#include <raylib.h>
#include <raylib-cpp.hpp>

Building::Building() {
  color = YELLOW;
}

void Building::Render(raylib::Vector2 pos, bool hovered) {
  DrawRectangle(pos.x-5, pos.y-5, 10, 10, color);
}
