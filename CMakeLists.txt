# Корневой CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(Sand2D VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================
# БИБЛИОТЕКА Sand2D
# ============================================
add_library(Sand2D STATIC
    src/World.cpp
    src/PhysicsSystem.cpp
)

# Где искать заголовки (добавляем include в путь)
target_include_directories(Sand2D PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============================================
# ПРИМЕР: sandbox
# ============================================
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(FallingSandExample
    examples/sandbox/main.cpp
    examples/sandbox/Renderer.cpp
)

target_link_libraries(FallingSandExample
    Sand2D
    OpenGL::GL
    glfw
    GLEW::GLEW
)

target_include_directories(FallingSandExample PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include  # ← для <Sand2D/...>
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

# ============================================
# УСТАНОВКА
# ============================================
install(TARGETS Sand2D DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)