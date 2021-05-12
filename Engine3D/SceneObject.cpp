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
#include "SceneObject.hpp"


#include <GL/glew.h>
#include <SDL2/SDL_ttf.h>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Macros.hpp"

namespace Engine3D
{
    void SceneObject::draw()
    {
        if(m_mesh.empty())
        {
            return;
        }

        glPushMatrix();

        glTranslatef(m_pos.x, m_pos.y, m_pos.z);
        glMultMatrixf(glm::value_ptr(m_rot));
        glScalef(m_scale.x, m_scale.y, m_scale.z);
        
        m_mesh.draw();

        glPopMatrix();
    }

    Triangle SceneObject::constructTriangle(u32 vert_index_start)
    {
        Triangle raw_triangle = m_mesh.constructTriangle(vert_index_start);

        raw_triangle.p1 = glm::vec3(m_rot * glm::vec4(raw_triangle.p1, 1.0f));
        raw_triangle.p2 = glm::vec3(m_rot * glm::vec4(raw_triangle.p2, 1.0f));
        raw_triangle.p3 = glm::vec3(m_rot * glm::vec4(raw_triangle.p3, 1.0f));

        raw_triangle.p1 = m_pos + raw_triangle.p1 * m_scale;
        raw_triangle.p2 = m_pos + raw_triangle.p2 * m_scale;
        raw_triangle.p3 = m_pos + raw_triangle.p3 * m_scale;
        
        return raw_triangle;
    }

    /**
     * extract vertex
     */
    std::optional<Vertex> SceneObject::getVertex(u32 i)
    {
        if(i >= vertices()->size())
        {
            return {};
        }

        Vertex v = (*vertices())[i];

        v.pos = glm::vec3(m_rot * glm::vec4(v.pos, 1.0f));
        v.nor = glm::vec3(m_rot * glm::vec4(v.nor, 1.0f));

        v.pos = m_pos + v.pos * m_scale;

        return v;
    }
};