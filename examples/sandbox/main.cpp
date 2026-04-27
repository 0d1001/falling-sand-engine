#include <Sand2D/Sand2D.h>
#include "Renderer.h"
#include <cstdlib>
#include <ctime>

int main() {
    Sand2D::ParticleRegistry registry;
    Sand2D::registerSand2DParticles(registry);

    Sand2D::ParticleDefinition lava;
    lava.name = "Lava";
    lava.state = Sand2D::PhysicalState::Liquid;
    lava.density = 3100.0f;
    lava.color = 0xFF4500FF;
    registry.registerParticle(lava);

    Sand2D::ParticleDefinition smoke;
    smoke.name = "Smoke";
    smoke.state = Sand2D::PhysicalState::Gas;
    smoke.density = 0.6f;
    smoke.color = 0x88888888;
    registry.registerParticle(smoke);
    
    Sand2D::World world(200, 150, registry);
    Sand2D::PhysicsSystem physics;
    
    Renderer renderer(1280, 720, "Sandbox - Physics Demo", registry);
    
    Sand2D::ParticleId sandId = registry.findId("Sand");
    Sand2D::ParticleId waterId = registry.findId("Water");
    Sand2D::ParticleId wallId = registry.findId("Wall");
    Sand2D::ParticleId oilId = registry.findId("Oil");
    Sand2D::ParticleId emptyId = Sand2D::ParticleRegistry::Empty;
    
    for (int x = 95; x < 105; ++x)
        for (int y = 0; y < 20; ++y)
            world.setParticle(x, y, sandId);
    
    for (int y = 0; y < 30; ++y)
        world.setParticle(30, y, waterId);
    
    for (int y = 100; y < 150; ++y)
        world.setParticle(170, y, wallId);
    
    Sand2D::ParticleId currentBrush = sandId;
    
    while (renderer.isOpen()) {
        renderer.handleEvents();
        
        if (glfwGetKey(renderer.getWindow(), GLFW_KEY_1) == GLFW_PRESS)
            currentBrush = sandId;
        if (glfwGetKey(renderer.getWindow(), GLFW_KEY_2) == GLFW_PRESS)
            currentBrush = waterId;
        if (glfwGetKey(renderer.getWindow(), GLFW_KEY_3) == GLFW_PRESS)
            currentBrush = wallId;
        if (glfwGetKey(renderer.getWindow(), GLFW_KEY_4) == GLFW_PRESS)
            currentBrush = oilId;
        if (glfwGetKey(renderer.getWindow(), GLFW_KEY_5) == GLFW_PRESS)
            currentBrush = registry.findId("Lava");;
        if (glfwGetKey(renderer.getWindow(), GLFW_KEY_6) == GLFW_PRESS)
            currentBrush = registry.findId("Smoke");;
        
        int x, y;
        renderer.getMouseWorldPosition(x, y);
        
        if (glfwGetMouseButton(renderer.getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            if (world.isInside(x, y) && world.getParticleId(x, y) == emptyId) {
                world.setParticle(x, y, currentBrush);
                renderer.markDirty(x, y);
            }
        }
        
        if (glfwGetMouseButton(renderer.getWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            if (world.isInside(x, y) && world.getParticleId(x, y) != emptyId) {
                world.setParticle(x, y, emptyId);
                renderer.markDirty(x, y);
            }
        }
        
        physics.update(world);
        renderer.render(world);
    }
    
    return 0;
}