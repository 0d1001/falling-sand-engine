#pragma once

#include <SFML/Graphics.hpp>
#include "World.h"

class Renderer
{
public:
    Renderer(int width, int height, const std::string& title);
    ~Renderer();

    void render(World& world);
    void handleEvents();
    
    bool isOpen() const { return m_window.isOpen(); }
    void close() { m_window.close(); }
    
    sf::Vector2i getMouseWorldPosition() const;
private:
    sf::RenderWindow m_window;
    int m_pixelSize = 4;
    sf::RectangleShape m_pixelRect;
};