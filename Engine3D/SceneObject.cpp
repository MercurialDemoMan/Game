#include "SceneObject.hpp"


#include <GL/glew.h>
#include <SDL2/SDL_ttf.h>

#include <glm/gtx/rotate_vector.hpp>

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
        
        glRotatef(m_rot.x, 1.0f, 0.0f, 0.0f);
        glRotatef(m_rot.y, 0.0f, 1.0f, 0.0f);
        glRotatef(m_rot.z, 0.0f, 0.0f, 1.0f);
         
        glScalef(m_scale.x, m_scale.y, m_scale.z);
        
        m_mesh.draw();

        glPopMatrix();
    }

    Triangle SceneObject::constructTriangle(u32 vert_index_start)
    {
        Triangle raw_triangle = m_mesh.constructTriangle(vert_index_start);

        //TODO: this is wrong
        raw_triangle.p1 = glm::rotate(raw_triangle.p1, static_cast<float>(m_rot.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
        raw_triangle.p1 = glm::rotate(raw_triangle.p1, static_cast<float>(m_rot.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
        raw_triangle.p1 = glm::rotate(raw_triangle.p1, static_cast<float>(m_rot.z * M_PI / 180.0f), glm::vec3(0, 0, 1));

        raw_triangle.p2 = glm::rotate(raw_triangle.p2, static_cast<float>(m_rot.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
        raw_triangle.p2 = glm::rotate(raw_triangle.p2, static_cast<float>(m_rot.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
        raw_triangle.p2 = glm::rotate(raw_triangle.p2, static_cast<float>(m_rot.z * M_PI / 180.0f), glm::vec3(0, 0, 1));

        raw_triangle.p3 = glm::rotate(raw_triangle.p3, static_cast<float>(m_rot.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
        raw_triangle.p3 = glm::rotate(raw_triangle.p3, static_cast<float>(m_rot.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
        raw_triangle.p3 = glm::rotate(raw_triangle.p3, static_cast<float>(m_rot.z * M_PI / 180.0f), glm::vec3(0, 0, 1));

        raw_triangle.p1 = m_pos + raw_triangle.p1 * m_scale;
        raw_triangle.p2 = m_pos + raw_triangle.p2 * m_scale;
        raw_triangle.p3 = m_pos + raw_triangle.p3 * m_scale;
        return raw_triangle;
    }
};