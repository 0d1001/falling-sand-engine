#pragma once

#include <vector>

namespace Sand2D {

enum class ParticleType {
    Empty,
    Sand,
    Water,
    Wall
};

struct Particle {
    ParticleType type = ParticleType::Empty;
};

class World
{
public:
    World(int width, int height);
    
    Particle& getParticle(int x, int y);
    void setParticle(int x, int y, ParticleType type);
    bool isInside(int x, int y) const;
    
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    
private:
    int m_width;
    int m_height;
    std::vector<Particle> m_grid;
};

}