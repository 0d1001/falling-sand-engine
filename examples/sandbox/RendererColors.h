#pragma once

#include <Sand2D/ParticleTypes.h>
#include <unordered_map>
#include <array>

class RendererColors {
public:
    static RendererColors& getInstance() {
        static RendererColors instance;
        return instance;
    }
    
    void setColor(Sand2D::ParticleType type, float r, float g, float b) {
        m_colors[type] = {r, g, b};
    }
    
    void getColor(Sand2D::ParticleType type, float& r, float& g, float& b) const {
        auto it = m_colors.find(type);
        if (it != m_colors.end()) {
            r = it->second[0];
            g = it->second[1];
            b = it->second[2];
        } else {
            r = g = b = 1.0f;
        }
    }
    
private:
    std::unordered_map<Sand2D::ParticleType, std::array<float, 3>> m_colors;
};

inline void initRendererColors() {
    auto& colors = RendererColors::getInstance();
    colors.setColor(Sand2D::ParticleType::Sand, 194.0f/255.0f, 178.0f/255.0f, 128.0f/255.0f);
    colors.setColor(Sand2D::ParticleType::Water, 64.0f/255.0f, 164.0f/255.0f, 223.0f/255.0f);
    colors.setColor(Sand2D::ParticleType::Wall, 100.0f/255.0f, 100.0f/255.0f, 100.0f/255.0f);
    colors.setColor(Sand2D::ParticleType::Oil, 139.0f/255.0f, 69.0f/255.0f, 19.0f/255.0f);
}