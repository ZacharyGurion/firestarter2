#ifndef ENEMYBUILDING_H
#define ENEMYBUILDING_H

#include "building.h"
#include <raylib.h>
#include <raylib-cpp.hpp>

class EnemyBuilding : public Building {
  public:
    EnemyBuilding();
    void Render(raylib::Vector2 pos, bool hovered = false) override;
};

#endif
