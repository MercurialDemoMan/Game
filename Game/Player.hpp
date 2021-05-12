/*
This file is part of Game.

Game is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Game is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Game.  If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once

#include <Engine3D/Camera.hpp>
#include <Engine3D/Types.hpp>
#include <Engine3D/Shader.hpp>

#include "Object.hpp"

/**
 * manage player
 */
class Player final : public Object
{
public:
    
    static constexpr const float DefaultReload = 30.0f;
    
    /**
     * constructors
     */
    Player(const Engine3D::Shader& object_shader) : Object(glm::vec3(0), "data/objects/ship.obj") 
    {
        this->scale() = this->dims() = glm::vec3(1);
        this->hitbox() = Object::HitboxType::Mesh;
        this->setupVertexAttributes(object_shader);
    }
    
    /**
     * constants
     */
    constexpr static float Acceleration  = 0.01f;
    constexpr static float Decceleration = 0.15f;
    constexpr static float MaxSpeed      = 0.5f;

    /**
     * getters
     */
    const glm::vec3&  getDir() { return m_camera.getDir(); }
    Engine3D::Camera& getCam() { return m_camera; }
    /**
     * setters
     */

    void setDir(const glm::vec3& dir)  { m_camera.setDir(dir); }

    void reset()
    {
        pos() = (glm::vec3(0));
        setDir(glm::vec3(0, 0, 1));
        mov() = glm::vec3(0);
    }

    float& gun_reload() { return m_gun_reload; }
    float& speed() { return m_speed; }

private:
    Engine3D::Camera m_camera;

    float m_gun_reload { 0 };
    float m_speed { 0 };
};