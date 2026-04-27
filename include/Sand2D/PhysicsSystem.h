#pragma once

#include <Sand2D/World.h>

namespace Sand2D {

class PhysicsSystem {
public:
    void update(World& world);
private:
    void updatePowder(World& world, int x, int y);
    void updateLiquid(World& world, int x, int y);
    void updateGas(World& world, int x, int y);
    
    bool tryMove(World& world, int fromX, int fromY, int toX, int toY);
};

}