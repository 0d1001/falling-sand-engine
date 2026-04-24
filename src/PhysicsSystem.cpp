#include "PhysicsSystem.h"

void PhysicsSystem::update(World& world)
{
    for (int y = world.getHeight() - 2; y >= 0; y--)
    {
        for (int x = 0; x < world.getWidth(); x++)
        {
            ParticleType particleType = world.getParticle(x, y).type;

            switch(particleType)
            {
                case ParticleType::Sand:
                    updateSand(world, x, y);
                    break;
                case ParticleType::Water:
                    updateWater(world, x, y);
                    break;
                default:
                    break;
            }
        }
    }
}

void PhysicsSystem::updateSand(World& world, int x, int y)
{
    if (tryMove(world, x, y, x, y + 1)) return;
}

void PhysicsSystem::updateWater(World& world, int x, int y)
{
    if (tryMove(world, x, y, x, y + 1)) return;
}
    
bool PhysicsSystem::tryMove(World& world, int fromX, int fromY, int toX, int toY)
{
    if (world.getParticle(toX, toY).type == ParticleType::Empty) {
        ParticleType temp = world.getParticle(fromX, fromY).type;

        world.setParticle(fromX, fromY, ParticleType::Empty);
        world.setParticle(toX, toY, temp);
        return true;
    }
    return false;
}