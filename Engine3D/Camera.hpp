/*
This file is part of Engine3D.

Engine3D is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Engine3D is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Engine3D.  If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Macros.hpp"

namespace Engine3D
{
    /**
     * structure for calculating view and rotation matrix
     */
    class Camera
    {
    public:
        
        /**
        ¨* constructors
        */
        Camera(){}
        Camera(glm::vec3 pos) : m_pos(pos) {}
        Camera(glm::vec3 pos, glm::vec3 dir) : m_pos(pos), m_dir(glm::normalize(dir)) {}
        
        /**
         * getters
         */
        const glm::vec3& getPos() const { return m_pos; }
        const glm::vec3& getDir() const { return m_dir; }
        /**
         * setters
         */
        void setPos(const glm::vec3& pos)  { m_rot_needs_update = m_view_needs_update = true; m_pos = pos; }
        void setX(float x)                 { m_rot_needs_update = m_view_needs_update = true; m_pos.x = x; }
        void setY(float y)                 { m_rot_needs_update = m_view_needs_update = true; m_pos.y = y; }
        void setZ(float z)                 { m_rot_needs_update = m_view_needs_update = true; m_pos.z = z; }
        void addPos(const glm::vec3& pos)  { m_rot_needs_update = m_view_needs_update = true; m_pos += pos; }
        void setDir(const glm::vec3& dir)  { m_rot_needs_update = m_view_needs_update = true; m_dir = glm::normalize(dir); }

        /**
         * rotate the camera around a axis
         */
        void rotate(glm::vec3& axis, float angle_in_rad);
        void rotate(glm::vec3&& axis, float angle_in_rad);
        
        /**
         * calculate the rotation matrix
         */
        const glm::mat4& getRotationMatrix();
        
        /**
         * calculate the view matrix
         */
        const glm::mat4& getViewMatrix();
        
        /**
         * access field of view
         */
        void  setFov(float fov_in_deg) { m_fov = fov_in_deg; }
        float getFov()                 const { return m_fov; }
        float getFovInRad()            const { return static_cast<float>(m_fov * 180.0f / M_PI); }
        
    private:
    
        glm::vec3 m_pos { 0 };
        glm::vec3 m_dir { 0, 0, 1 };
        
        float     m_fov { 60 };
        
        bool      m_rot_needs_update { true };
        glm::mat4 m_rot_matrix;
        bool      m_view_needs_update { true };
        glm::mat4 m_view_matrix;
    };
};