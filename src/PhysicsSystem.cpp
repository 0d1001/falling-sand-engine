#include <Sand2D/PhysicsSystem.h>
#include <cstdlib>

namespace Sand2D {

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
    int dx = (rand() & 2) - 1;

    if (tryMove(world, x, y, x, y + 1)) return;

    if (tryMove(world, x, y, x + dx, y + 1)) return;
    if (tryMove(world, x, y, x - dx, y + 1)) return;
}

void PhysicsSystem::updateWater(World& world, int x, int y)
{
    int dx = (rand() & 2) - 1;
    if (tryMove(world, x, y, x, y + 1)) return;

    if (tryMove(world, x, y, x + dx, y)) return;
    if (tryMove(world, x, y, x - dx, y)) return;
    

    if (tryMove(world, x, y, x + dx, y + 1)) return;
    if (tryMove(world, x, y, x - dx, y + 1)) return;
}
    
bool PhysicsSystem::tryMove(World& world, int fromX, int fromY, int toX, int toY)
{
    if (!world.isInside(toX, toY + 1)) return false;

    Particle& from = world.getParticle(fromX, fromY);
    Particle& to = world.getParticle(toX, toY);
    
    if (to.type == ParticleType::Empty || to.type == ParticleType::Water and from.type == ParticleType::Sand)
    {
        std::swap(from.type, to.type);
        return true;
    }

    return false;
}

}