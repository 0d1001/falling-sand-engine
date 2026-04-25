#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Sand2D/World.h>
#include <string>
#include <vector>

class Renderer
{
public:
    Renderer(int width, int height, const std::string& title);
    ~Renderer();

    void render(Sand2D::World& world);
    void handleEvents();
    
    bool isOpen() const { return !glfwWindowShouldClose(m_window); }
    void close() { glfwSetWindowShouldClose(m_window, true); }
    GLFWwindow* getWindow() { return m_window; }
    
    void getMouseWorldPosition(Sand2D::World& world, int& x, int& y) const;
    
private:
    void setupBuffers();
    void rebuildVertexArray(Sand2D::World& world);
    void compileShaders();
    void updateViewport();
    
    // OpenGL objects
    GLuint m_vao;           // Vertex Array Object
    GLuint m_vbo;           // Vertex Buffer Object
    GLuint m_shaderProgram;
    
    // Window and world properties
    GLFWwindow* m_window;
    int m_windowWidth;
    int m_windowHeight;
    int m_viewportX;
    int m_viewportY;
    int m_viewportWidth;
    int m_viewportHeight;
    
    std::vector<float> m_vertices;  // Temporary vertex storage
};