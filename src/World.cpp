#include <Sand2D/World.h>

namespace Sand2D {

World::World(int width, int height) 
    : m_width(width), m_height(height)
    , m_grid(width * height)
{
}

ParticleInstance& World::getParticle(int x, int y)
{
    return m_grid[y * m_width + x];
}

void World::setParticle(int x, int y, ParticleType type)
{
    if (isInside(x, y))
        m_grid[y * m_width + x].type = type;
}

bool World::isInside(int x, int y) const
{
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

}