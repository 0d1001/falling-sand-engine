#include "Renderer.h"
#include "RendererColors.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>

// Vertex shader source
const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec3 aColor;
    
    out vec3 vertexColor;
    uniform vec2 u_offset;
    
    void main() {
        vertexColor = aColor;
        gl_Position = vec4(aPos.x + u_offset.x, aPos.y + u_offset.y, 0.0, 1.0);
    }
)";

// Fragment shader source
const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 vertexColor;
    out vec4 FragColor;
    
    void main() {
        vec3 color = vertexColor;
        
        color = color * 1.15;
        
        float gray = (color.r + color.g + color.b) / 3.0;
        color = mix(vec3(gray), color, 1.3);
        
        color = min(color, 1.0);
        
        FragColor = vec4(color, 1.0);
    }
)";
Renderer::Renderer(int width, int height, const std::string& title)
    : m_windowWidth(width), m_windowHeight(height),
      m_viewportX(0), m_viewportY(0), m_viewportWidth(0), m_viewportHeight(0)
{
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }
    
    // Configure GLFW for OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);  // Allow window resizing
    
    // Create window
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    
    glfwMakeContextCurrent(m_window);
    
    // Set window resize callback
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int w, int h) {
        Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
        renderer->updateViewport();
    });
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(-1);
    }
    
    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    compileShaders();
    setupBuffers();
    updateViewport();
}

Renderer::~Renderer()
{
    // Cleanup OpenGL resources
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteProgram(m_shaderProgram);
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Renderer::updateViewport()
{
    // Get current framebuffer size
    glfwGetFramebufferSize(m_window, &m_windowWidth, &m_windowHeight);
    
    // Set viewport to full window (pixels will stretch)
    m_viewportX = 0;
    m_viewportY = 0;
    m_viewportWidth = m_windowWidth;
    m_viewportHeight = m_windowHeight;
    
    glViewport(m_viewportX, m_viewportY, m_viewportWidth, m_viewportHeight);
}

void Renderer::compileShaders()
{
    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    
    // Check vertex shader compilation
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed:\n" << infoLog << std::endl;
    }
    
    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    
    // Check fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed:\n" << infoLog << std::endl;
    }
    
    // Link shaders into program
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);
    
    // Check linking
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
    }
    
    // Cleanup shader objects (no longer needed after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Renderer::setupBuffers()
{
    // Create VAO and VBO
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    
    // Position attribute (2 floats: x, y)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute (3 floats: r, g, b)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Renderer::rebuildVertexArray(Sand2D::World& world)
{
    m_vertices.clear();
    
    // Calculate cell size in OpenGL coordinates based on window size
    // This makes pixels stretch to fill the entire window
    float worldWidth = world.getWidth();
    float worldHeight = world.getHeight();
    
    // Scale factors to stretch pixels across the whole window
    float scaleX = 2.0f / worldWidth;
    float scaleY = 2.0f / worldHeight;
    
    for (int y = 0; y < world.getHeight(); ++y)
    {
        for (int x = 0; x < world.getWidth(); ++x)
        {
            Sand2D::ParticleType type = world.getParticle(x, y).type;
            if (type == Sand2D::ParticleType::Empty) continue;
            
            // Calculate RGB color based on particle type
            float r, g, b;

            RendererColors::getInstance().getColor(type, r, g, b);
            
            // Calculate cell position in OpenGL coordinate space (-1 to 1)
            // Invert Y coordinate because OpenGL origin is bottom-left
            float px = -1.0f + x * scaleX;                    // Left edge X
            float py = 1.0f - y * scaleY;               // Top edge Y (inverted)
            float pw = scaleX;                                 // Cell width
            float ph = scaleY;                                 // Cell height
            
            // Two triangles to form a quad (6 vertices)
            // Triangle 1: top-left, top-right, bottom-right
            m_vertices.push_back(px); m_vertices.push_back(py);     // Top-left
            m_vertices.push_back(r); m_vertices.push_back(g); m_vertices.push_back(b);
            
            m_vertices.push_back(px + pw); m_vertices.push_back(py); // Top-right
            m_vertices.push_back(r); m_vertices.push_back(g); m_vertices.push_back(b);
            
            m_vertices.push_back(px + pw); m_vertices.push_back(py - ph); // Bottom-right
            m_vertices.push_back(r); m_vertices.push_back(g); m_vertices.push_back(b);
            
            // Triangle 2: top-left, bottom-right, bottom-left
            m_vertices.push_back(px); m_vertices.push_back(py);     // Top-left
            m_vertices.push_back(r); m_vertices.push_back(g); m_vertices.push_back(b);
            
            m_vertices.push_back(px + pw); m_vertices.push_back(py - ph); // Bottom-right
            m_vertices.push_back(r); m_vertices.push_back(g); m_vertices.push_back(b);
            
            m_vertices.push_back(px); m_vertices.push_back(py - ph); // Bottom-left
            m_vertices.push_back(r); m_vertices.push_back(g); m_vertices.push_back(b);
        }
    }
}

void Renderer::render(Sand2D::World& world)
{
    // Update vertex data based on current world state
    rebuildVertexArray(world);
    
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Use our shader program
    glUseProgram(m_shaderProgram);
    
    // Upload vertex data to GPU
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_DYNAMIC_DRAW);
    
    // Draw all triangles
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 5);
    
    // Swap buffers (double buffering)
    glfwSwapBuffers(m_window);
}

void Renderer::handleEvents()
{
    glfwPollEvents();
    
    // Close window on ESC key
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
}

void Renderer::getMouseWorldPosition(Sand2D::World& world, int& x, int& y) const
{
    double mouseX, mouseY;
    glfwGetCursorPos(m_window, &mouseX, &mouseY);
    
    // Get current window size
    int windowWidth, windowHeight;
    glfwGetWindowSize(m_window, &windowWidth, &windowHeight);
    
    // Convert screen coordinates to world grid coordinates
    // Mouse Y is inverted because OpenGL origin is bottom-left
    float worldX = (mouseX / windowWidth) * world.getWidth();
    float worldY = (mouseY / windowHeight) * world.getHeight();
    
    x = static_cast<int>(worldX);
    y = static_cast<int>(worldY);
    
    // Clamp to world boundaries
    x = std::max(0, std::min(x, world.getWidth() - 1));
    y = std::max(0, std::min(y, world.getHeight() - 1));
}