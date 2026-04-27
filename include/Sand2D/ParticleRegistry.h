#pragma once

#include <Sand2D/ParticleTypes.h>
#include <unordered_map>

namespace Sand2D {

class ParticleRegistry {
public:
    static ParticleRegistry& getInstance() {
        static ParticleRegistry instance;
        return instance;
    }
    
    void registerParticle(ParticleType type, const ParticleProps& props) {
        m_props[type] = props;
    }
    
    const ParticleProps& getProps(ParticleType type) const {
        static ParticleProps empty;
        auto it = m_props.find(type);
        return (it != m_props.end()) ? it->second : empty;
    }
    
    State getState(ParticleType type) const {
        return getProps(type).state;
    }
    
    float getDensity(ParticleType type) const {
        return getProps(type).density;
    }
    
private:
    std::unordered_map<ParticleType, ParticleProps> m_props;
};

////////////////STANDART PARTICLES////////////////
inline void registerSand2DParticles() {
    auto& registry = ParticleRegistry::getInstance();
    
    ParticleProps sand;
    sand.state = State::Powder;
    sand.density = 1600.0f;
    sand.viscosity = 0.0f;
    sand.restitution = 0.3f;
    sand.friction = 0.6f;
    registry.registerParticle(ParticleType::Sand, sand);
    
    ParticleProps water;
    water.state = State::Liquid;
    water.density = 1000.0f;
    water.viscosity = 0.01f;
    water.restitution = 0.1f;
    water.friction = 0.3f;
    registry.registerParticle(ParticleType::Water, water);
    
    ParticleProps oil;
    oil.state = State::Liquid;
    oil.density = 800.0f;
    oil.viscosity = 0.05f;
    registry.registerParticle(ParticleType::Oil, oil);
}

}