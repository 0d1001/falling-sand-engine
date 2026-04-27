#include <Sand2D/PhysicsSystem.h>
#include <Sand2D/ParticleRegistry.h>
#include <cstdlib>

namespace Sand2D {

void PhysicsSystem::update(World& world) {
    for (int y = world.getHeight() - 1; y >= 0; y--) {
        for (int x = 0; x < world.getWidth(); x++) {
            ParticleType type = world.getParticle(x, y).type;
            if (type == ParticleType::Empty) continue;
            
            State state = ParticleRegistry::getInstance().getState(type);
            
            switch(state) {
                case State::Powder: updatePowder(world, x, y); break;
                case State::Liquid: updateLiquid(world, x, y); break;
                default: break;
            }
        }
    }
}

void PhysicsSystem::updatePowder(World& world, int x, int y)
{
    int dx = (rand() & 2) - 1;

    if (tryMove(world, x, y, x, y + 1)) return;

    if (tryMove(world, x, y, x + dx, y + 1)) return;
    if (tryMove(world, x, y, x - dx, y + 1)) return;
}

void PhysicsSystem::updateLiquid(World& world, int x, int y)
{
    int dx = (rand() & 2) - 1;
    if (tryMove(world, x, y, x, y + 1)) return;

    if (tryMove(world, x, y, x + dx, y)) return;
    if (tryMove(world, x, y, x - dx, y)) return;
    

    if (tryMove(world, x, y, x + dx, y + 1)) return;
    if (tryMove(world, x, y, x - dx, y + 1)) return;
}
    
bool PhysicsSystem::tryMove(World& world, int fromX, int fromY, int toX, int toY) {
    if (!world.isInside(toX, toY)) return false;
    
    ParticleInstance& from = world.getParticle(fromX, fromY);
    ParticleInstance& to = world.getParticle(toX, toY);
    
    const auto& fromProps = ParticleRegistry::getInstance().getProps(from.type);
    const auto& toProps = ParticleRegistry::getInstance().getProps(to.type);
    
    if (to.type == ParticleType::Empty || (toProps.density < fromProps.density && toProps.state != State::Solid)) {
        std::swap(from.type, to.type);
        //std::swap(from.temperature, to.temperature);
        return true;
    }
    
    return false;
}

}