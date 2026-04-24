#pragma once
#include "World.h"

class PhysicsSystem {
public:
    void update(World& world);
private:
    void updateSand(World& world, int x, int y);
    void updateWater(World& world, int x, int y);
    
    bool tryMove(World& world, int fromX, int fromY, int toX, int toY);
};