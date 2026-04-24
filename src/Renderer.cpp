#include "Renderer.h"

Renderer::Renderer(int width, int height, const std::string& title) 
    : m_window(sf::VideoMode(width, height), title)
{
    m_pixelRect.setSize(sf::Vector2f(m_pixelSize, m_pixelSize));
}

Renderer::~Renderer() 
{
    if (m_window.isOpen()) 
        m_window.close();
}

void Renderer::render(World& world)
{
    m_window.clear(sf::Color::Black);
    
    for (int y = 0; y < world.getHeight(); ++y)
    {
        for (int x = 0; x < world.getWidth(); ++x)
        {
            ParticleType type = world.getParticle(x, y).type;
            if (type != ParticleType::Empty)
            {
                sf::Color color;
                switch (type)
                {
                    case ParticleType::Sand:  color = sf::Color(194, 178, 128); break;
                    case ParticleType::Water: color = sf::Color(64, 164, 223); break;
                    case ParticleType::Wall:  color = sf::Color(100, 100, 100); break;
                    default: color = sf::Color::Magenta; break;
                }
                
                m_pixelRect.setPosition(x * m_pixelSize, y * m_pixelSize);
                m_pixelRect.setFillColor(color);
                m_window.draw(m_pixelRect);
            }
        }
    }
    
    m_window.display();
}

void Renderer::handleEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_window.close();
        
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
                m_window.close();
        }
    }
}

sf::Vector2i Renderer::getMouseWorldPosition() const
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
    return sf::Vector2i(mousePos.x / m_pixelSize, mousePos.y / m_pixelSize);
}