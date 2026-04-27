#pragma once

namespace Sand2D {

enum class ParticleType {
    Empty,
    Sand,
    Water,
    Wall,
    Oil
};

enum class State {
    Solid,
    Liquid,
    Gas,
    Powder
};

struct ParticleProps {
    State state = State::Solid;
    float density = 0.0f;           // kg/m³
    float viscosity = 0.0f;
    float restitution = 0.0f;
    float friction = 0.0f;
    float thermalConductivity = 0.0f;
    float heatCapacity = 0.0f;
    float meltingPoint = 1000.0f;
    float boilingPoint = 2000.0f;
    ParticleType nextMaterial = ParticleType::Empty;
};

}