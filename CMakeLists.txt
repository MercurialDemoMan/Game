cmake_minimum_required(VERSION 3.7)

project(Game)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -O2 -g")

add_executable(Game 
Game/main.cpp
Game/GameLogic.cpp 
Game/Level.cpp 
Game/Light.cpp 
Game/Object.cpp 
Game/Player.cpp)

add_library(Engine3D STATIC
Engine3D/BillboardObject.cpp
Engine3D/Camera.cpp
Engine3D/Canvas.cpp
Engine3D/Collision.cpp
Engine3D/Cubemap.cpp
Engine3D/FBObject.cpp
Engine3D/File.cpp
Engine3D/FPSLimiter.cpp
Engine3D/Game.cpp
Engine3D/Gamepad.cpp
Engine3D/JSONDocument.cpp
Engine3D/Mesh.cpp
Engine3D/Music.cpp
Engine3D/Save.cpp
Engine3D/SceneObject.cpp
Engine3D/Shader.cpp
Engine3D/Sound.cpp
Engine3D/Sprite.cpp
Engine3D/System.cpp
Engine3D/Text.cpp
Engine3D/Texture.cpp
Engine3D/Random.cpp)

target_include_directories(Game PUBLIC ${CMAKE_SOURCE_DIR})

target_link_libraries(Game -lSDL2)
target_link_libraries(Game -lSDL2_image)
target_link_libraries(Game -lSDL2_ttf)
target_link_libraries(Game -lSDL2_mixer)
target_link_libraries(Game ${GLEW_LIBRARIES})
target_link_libraries(Game ${OPENGL_LIBRARIES})
target_link_libraries(Game Engine3D)
