#include <Sand2D/PhysicsSystem.h>
#include <Sand2D/ParticleRegistry.h>
#include <cstdlib>
#include <random>
#include <chrono>

namespace Sand2D {

static std::mt19937& getRng() {
    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    return rng;
}

void PhysicsSystem::update(World& world) {
    int totalCells = world.getWidth() * world.getHeight();
    m_movedThisFrame.assign(totalCells, false);
    m_pendingMoves.clear();
    
    // Scan from bottom to top for realistic falling behavior
    

    for (int y = world.getHeight() - 1; y >= 0; y--) {
        for (int x = 0; x < world.getWidth(); x++) {
            bool leftToRight = (x % 2 == 0);

            int idx;
            if (leftToRight) idx = y * world.getWidth() + x;
            else idx = y * world.getWidth() + (world.getWidth() - x);
            
            // Skip if this particle already moved this frame
            if (m_movedThisFrame[idx]) continue;
            
            ParticleId id = world.getParticleId(x, y);
            if (id == ParticleRegistry::Empty) continue;
            
            int oldX = x, oldY = y;
            
            updateParticle(world, x, y);
            
            // Check if the particle actually moved to a new position
            if (world.getParticleId(oldX, oldY) == ParticleRegistry::Empty &&
                world.getParticleId(x, y) != ParticleRegistry::Empty) {
                int newIdx = y * world.getWidth() + x;
                m_movedThisFrame[newIdx] = true;
            }
        }
    }
}

void PhysicsSystem::updateParticle(World& world, int x, int y) {
    ParticleInstance& p = world.getParticle(x, y);
    const auto& def = world.getRegistry().get(p.id);
    
    switch (def.state) {
        case PhysicalState::Powder:
            updatePowder(world, x, y);
            break;
        case PhysicalState::Liquid:
            updateLiquid(world, x, y);
            break;
        case PhysicalState::Gas:
            updateGas(world, x, y);
            break;
        default:
            break;
    }
}

void PhysicsSystem::updatePowder(World& world, int x, int y) {
    auto& rng = getRng();
    std::uniform_int_distribution<int> dist(-1, 1);
    int dx = dist(rng);
    
    // Try to fall straight down
    if (tryMove(world, x, y, x, y + 1)) return;
    
    // Try to slide down diagonally (creates slope behavior)
    if (tryMove(world, x, y, x + dx, y + 1)) return;
    if (tryMove(world, x, y, x - dx, y + 1)) return;
}

void PhysicsSystem::updateLiquid(World& world, int x, int y) {
    auto& rng = getRng();
    std::uniform_int_distribution<int> dist(-1, 1);
    int dx = dist(rng);
    
    // Try to fall straight down
    if (tryMove(world, x, y, x, y + 1)) return;
    
    // Spread horizontally
    if (tryMove(world, x, y, x + dx, y)) return;
    if (tryMove(world, x, y, x - dx, y)) return;
    
    // Flow down diagonally
    if (tryMove(world, x, y, x + dx, y + 1)) return;
    if (tryMove(world, x, y, x - dx, y + 1)) return;
}

void PhysicsSystem::updateGas(World& world, int x, int y) {
    auto& rng = getRng();
    std::uniform_int_distribution<int> dist(-1, 1);
    int dx = dist(rng);

    // 5% chance for gas to dissipate (disappear) each frame
    std::uniform_int_distribution<int> dissipateChance(0, 9999);
    if (dissipateChance(rng) < 5) {  // 5% dissipation rate
        world.setParticle(x, y, ParticleRegistry::Empty);
        return;
    }
    
    // Gas rises upward
    if (tryMove(world, x, y, x, y - 1)) return;
    
    // Diffuse sideways
    if (tryMove(world, x, y, x + dx, y)) return;
    if (tryMove(world, x, y, x - dx, y)) return;
    
    // Rise diagonally
    if (tryMove(world, x, y, x + dx, y - 1)) return;
    if (tryMove(world, x, y, x - dx, y - 1)) return;
}
    
bool PhysicsSystem::tryMove(World& world, int fromX, int fromY, int toX, int toY) {
    // Boundary check
    if (!world.isInside(toX, toY)) return false;
    
    // Prevent multiple moves per frame (fixes gas teleportation)
    int toIdx = toY * world.getWidth() + toX;
    if (m_movedThisFrame[toIdx]) return false;
    
    ParticleInstance& from = world.getParticle(fromX, fromY);
    ParticleInstance& to = world.getParticle(toX, toY);
    const auto& fromDef = world.getRegistry().get(from.id);
    
    // Empty space - always movable
    if (to.id == ParticleRegistry::Empty) {
        std::swap(from.id, to.id);
        std::swap(from.temperature, to.temperature);
        
        int fromIdx = fromY * world.getWidth() + fromX;
        m_movedThisFrame[toIdx] = true;
        m_movedThisFrame[fromIdx] = true;
        return true;
    }
    
    const auto& toDef = world.getRegistry().get(to.id);
    
    // Cannot move through solid objects (walls)
    if (toDef.state == PhysicalState::Solid) return false;
    
    // Gases bubble up through liquids and powders
    if (fromDef.state == PhysicalState::Gas && 
        (toDef.state == PhysicalState::Liquid || toDef.state == PhysicalState::Powder)) {
        std::swap(from.id, to.id);
        std::swap(from.temperature, to.temperature);
        
        int fromIdx = fromY * world.getWidth() + fromX;
        m_movedThisFrame[toIdx] = true;
        m_movedThisFrame[fromIdx] = true;
        return true;
    }
    
    // Powders sink in liquids (sand falls through water/lava)
    if (fromDef.state == PhysicalState::Powder && toDef.state == PhysicalState::Liquid) {
        std::swap(from.id, to.id);
        std::swap(from.temperature, to.temperature);
        
        int fromIdx = fromY * world.getWidth() + fromX;
        m_movedThisFrame[toIdx] = true;
        m_movedThisFrame[fromIdx] = true;
        return true;
    }
    
    // Liquids cannot displace powders (lava cannot push sand)
    if (fromDef.state == PhysicalState::Liquid && toDef.state == PhysicalState::Powder) {
        return false;
    }
    
    // Liquid-liquid displacement based on density
    if (fromDef.state == PhysicalState::Liquid && toDef.state == PhysicalState::Liquid) {
        if (toDef.density < fromDef.density) {
            std::swap(from.id, to.id);
            std::swap(from.temperature, to.temperature);
            
            int fromIdx = fromY * world.getWidth() + fromX;
            m_movedThisFrame[toIdx] = true;
            m_movedThisFrame[fromIdx] = true;
            return true;
        }
        return false;
    }
    
    // Powder-powder displacement based on density
    if (fromDef.state == PhysicalState::Powder && toDef.state == PhysicalState::Powder) {
        if (toDef.density < fromDef.density) {
            std::swap(from.id, to.id);
            std::swap(from.temperature, to.temperature);
            
            int fromIdx = fromY * world.getWidth() + fromX;
            m_movedThisFrame[toIdx] = true;
            m_movedThisFrame[fromIdx] = true;
            return true;
        }
        return false;
    }
    
    // Default rule: less dense particles float above denser ones
    if (toDef.density < fromDef.density) {
        std::swap(from.id, to.id);
        std::swap(from.temperature, to.temperature);
        
        int fromIdx = fromY * world.getWidth() + fromX;
        m_movedThisFrame[toIdx] = true;
        m_movedThisFrame[fromIdx] = true;
        return true;
    }
    
    return false;
}

}