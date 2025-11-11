#include "enemybuilding.h"
#include <raylib.h>
#include <raylib-cpp.hpp>

EnemyBuilding::EnemyBuilding() : Building() {
  color = RED;
}

void EnemyBuilding::Render(raylib::Vector2 pos, bool hovered) {
  DrawRectangle(pos.x-5, pos.y-5, 10, 10, color);
}
