#pragma once

#include <Sand2D/ParticleTypes.h>
#include <vector>

namespace Sand2D {

struct ParticleInstance {
    ParticleType type = ParticleType::Empty;
    float temperature = 20.0f;
};

class World
{
public:
    World(int width, int height);
    
    ParticleInstance& getParticle(int x, int y);
    void setParticle(int x, int y, ParticleType type);
    bool isInside(int x, int y) const;
    
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    
private:
    int m_width;
    int m_height;
    std::vector<ParticleInstance> m_grid;
};

}