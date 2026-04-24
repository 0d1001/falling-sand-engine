cmake_minimum_required(VERSION 3.16)
project(falling-sand-engine LANGUAGES CXX)
include(FetchContent)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 2)
set(VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Due to bug in sfml's build system. It doesn't affect anything much so leaving it off
set(FLAGS "-Wno-unused-command-line-argument") 

set(CMAKE_C_FLAGS ${FLAGS})
set(CMAKE_CXX_FLAGS ${FLAGS})

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 11b73743c42cf7ecd7c596ba83fdbf1150ffa96c
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(SFML)

set(SOURCE_FILES 
    "src/main.cpp"
    "src/World.cpp"
    "src/PhysicsSystem.cpp"
    "src/Renderer.cpp"
)

add_executable(falling-sand-engine ${SOURCE_FILES})

target_link_libraries(falling-sand-engine PRIVATE sfml-graphics)
target_include_directories(falling-sand-engine PRIVATE src)

if(WIN32)
    add_custom_command(
        TARGET falling-sand-engine
        COMMENT "Copy OpenAL DLL"
        POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy 
            ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll 
            $<TARGET_FILE_DIR:falling-sand-engine>
        VERBATIM)
endif()

install(TARGETS falling-sand-engine)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)