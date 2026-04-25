#include <Sand2D/Sand2D.h>
#include "Renderer.h"
#include <cstdlib>
#include <ctime>

int main() 
{
    // Seed random number generator
    srand(static_cast<unsigned>(time(nullptr)));
    
    // Create world (200x150 grid)
    Sand2D::World world(200, 150);
    Sand2D::PhysicsSystem physicsSystem;
    Renderer renderer(1280, 720, "Noita-like Sandbox - OpenGL");
    
    // Test particles - Sand column in the middle
    for (int x = 95; x < 105; ++x)
        for (int y = 0; y < 20; ++y)
            world.setParticle(x, y, Sand2D::ParticleType::Sand);
    
    // Test particles - Water column on the left
    for (int y = 0; y < 30; ++y)
        world.setParticle(30, y, Sand2D::ParticleType::Water);
    
    // Test particles - Wall on the right
    for (int y = 100; y < 150; ++y)
        world.setParticle(170, y, Sand2D::ParticleType::Wall);
    
    // Current selected brush type
    Sand2D::ParticleType currentBrush = Sand2D::ParticleType::Sand;
    
    // Main game loop
    while (renderer.isOpen()) 
    {
        renderer.handleEvents();
        
        // Brush selection (keys 1-3)
        if (glfwGetKey(renderer.getWindow(), GLFW_KEY_1) == GLFW_PRESS)
            currentBrush = Sand2D::ParticleType::Sand;
        if (glfwGetKey(renderer.getWindow(), GLFW_KEY_2) == GLFW_PRESS)
            currentBrush = Sand2D::ParticleType::Water;
        if (glfwGetKey(renderer.getWindow(), GLFW_KEY_3) == GLFW_PRESS)
            currentBrush = Sand2D::ParticleType::Wall;
        
        // Left Mouse Button - Spawn particle
        if (glfwGetMouseButton(renderer.getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            int x, y;
            renderer.getMouseWorldPosition(world, x, y);
            if (world.isInside(x, y))
            {
                if(world.getParticle(x, y).type == Sand2D::ParticleType::Empty) world.setParticle(x, y, currentBrush);
            }
        }
        
        // Right Mouse Button - Delete particle (eraser)
        if (glfwGetMouseButton(renderer.getWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            int x, y;
            renderer.getMouseWorldPosition(world, x, y);
            if (world.isInside(x, y))
                world.setParticle(x, y, Sand2D::ParticleType::Empty);
        }
        
        // Update physics and render
        physicsSystem.update(world);
        renderer.render(world);
    }
    
    return 0;
}