cmake_minimum_required(VERSION 3.10)
project(OpenGLSandbox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} 
    OpenGL::GL 
    glfw
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} 
        opengl32
        glu32
    )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${OPENGL_INCLUDE_DIRS}
)