#include "World.h"
#include "PhysicsSystem.h"
#include "Renderer.h"

int main() 
{
    World world(200, 150);
    PhysicsSystem physicsSystem;
    Renderer renderer(800, 600, "noita-like-sandbox");
    
    for (int x = 95; x < 105; ++x)
    {
        for (int y = 0; y < 20; ++y)
        {
            world.setParticle(x, y, ParticleType::Sand);
        }
    }

    for (int y = 0; y < 15; ++y)
    {
        world.setParticle(55, y, ParticleType::Water);
    }
    
    for (int y = 100; y < 150; ++y)
    {
        world.setParticle(170, y, ParticleType::Wall);
    }

    ParticleType currentBrush = ParticleType::Sand;
    
    while (renderer.isOpen()) 
    {
        renderer.handleEvents();
        
        // ===== Control =====
        // Change brush type
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            currentBrush = ParticleType::Sand;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            currentBrush = ParticleType::Water;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            currentBrush = ParticleType::Wall;
        
        // Spawn Particles
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i pos = renderer.getMouseWorldPosition();
            
            if (world.isInside(pos.x, pos.y))
            {
                if (world.getParticle(pos.x, pos.y).type == ParticleType::Empty) world.setParticle(pos.x, pos.y, currentBrush);
            }
        }
        
        // RMB - Deleta
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            sf::Vector2i pos = renderer.getMouseWorldPosition();
            world.setParticle(pos.x, pos.y, ParticleType::Empty);
        }
        
        // ===== PHYSIC AND RENDER =====
        physicsSystem.update(world);
        renderer.render(world);
        
        sf::sleep(sf::milliseconds(16));  // 60 FPS
    }
    
    return 0;
}