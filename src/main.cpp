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
    
    for (int y = 0; y < 30; ++y)
    {
        world.setParticle(30, y, ParticleType::Water);
    }
    
    for (int y = 100; y < 150; ++y)
    {
        world.setParticle(170, y, ParticleType::Wall);
    }
    
    while (renderer.isOpen()) 
    {
        renderer.handleEvents();
        physicsSystem.update(world);
        renderer.render(world);

        sf::sleep(sf::milliseconds(20));  // 20ms = 50 FPS
    }
    
    return 0;
}