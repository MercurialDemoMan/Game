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

#include "Engine3D/Shader.hpp"
#include "Engine3D/SceneObject.hpp"

#include <functional>

class GameLogic;

class Object : public Engine3D::SceneObject
{
public:

    enum class HitboxType
    {
        None,
        Box,
        Cylinder,
        Ball,
        Mesh
    };

    /**
     * constructors
     */
    Object() : SceneObject() {}
    Object(glm::vec3 pos) : SceneObject(pos) {}
    Object(glm::vec3 pos, const char* mesh_id) : SceneObject(pos, mesh_id)
    {
        
    }
    Object(glm::vec3 pos, const std::string& mesh_id) : SceneObject(pos, mesh_id)
    {
        
    }
    Object(glm::vec3 pos, const char* mesh_id, HitboxType hitbox) : SceneObject(pos, mesh_id), m_hitbox_type(hitbox)
    {

    }
    Object(glm::vec3 pos, const std::string& mesh_id, HitboxType hitbox) : SceneObject(pos, mesh_id), m_hitbox_type(hitbox)
    {

    }
    Object(glm::vec3 pos, const char* mesh_id, HitboxType hitbox, std::function<void(void)> collision_trigger) : SceneObject(pos, mesh_id), m_hitbox_type(hitbox), m_collision_trigger(collision_trigger)
    {

    }
    Object(glm::vec3 pos, const std::string& mesh_id, HitboxType hitbox, std::function<void(void)> collision_trigger) :SceneObject(pos, mesh_id), m_hitbox_type(hitbox), m_collision_trigger(collision_trigger)
    {

    }

    /**
     * access attributes
     */
    HitboxType&               hitbox()                 { return m_hitbox_type; }
    std::function<void(void)> collision_trigger()      { return m_collision_trigger; }
    bool&                     has_collision_response() { return m_has_collision_response; }

    void setupVertexAttributes(const Engine3D::Shader& shader);

    virtual void update(GameLogic*, const float delta_time)
    {

    }

    glm::vec3& mov() { return m_mov; }
    glm::vec3& col() { return m_col; }
    bool& destroy_flag() { return m_destroy_flag; }

private:

    glm::vec3                 m_mov { 0 };
    glm::vec3                 m_col { 1 };

    bool                      m_destroy_flag { false };
    HitboxType                m_hitbox_type { HitboxType::None };
    std::function<void(void)> m_collision_trigger;
    bool                      m_has_collision_response { true };
};