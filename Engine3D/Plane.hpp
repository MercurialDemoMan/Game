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

#include "glm/glm.hpp"

namespace Engine3D
{
    class Plane
    {
    public:
        
        /**
         * constructors
         */
        Plane() {}
        Plane(glm::vec3& pos) : m_pos(pos) {}
        Plane(glm::vec3& pos, glm::vec3& normal) : 
            m_pos(pos),
            m_nor(normal)
        {
        }
        Plane(glm::vec3 pos, glm::vec3 normal, glm::vec2 dims) : 
            m_pos(pos),
            m_nor(normal),
            m_dims(dims)
        {
        }
        Plane(glm::vec3 pos, glm::vec3 normal, glm::vec2 dims, float scale) :
            m_pos(pos),
            m_nor(normal),
            m_dims(dims),
            m_scale(scale)
        {
        }
        
        /**
         * draw the plane
         */
         void draw();

        /**
         * access attributes
         */
         glm::vec3& pos()  { return m_pos; }
         glm::vec3& nor()  { return m_nor; }
         glm::vec2& dims() { return m_dims; }
         float& scale()    { return m_scale; }
        
    private:
    
        glm::vec3 m_pos  { 0 };
        glm::vec3 m_nor  { 0, 1, 0 };
        glm::vec2 m_dims { 1, 1 };
        float m_scale    { 1 };
    };
};