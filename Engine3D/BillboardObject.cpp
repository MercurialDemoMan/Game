#include "BillboardObject.hpp"

#include "Macros.hpp"

#include <GL/glew.h>
#include <SDL2/SDL_ttf.h>

#include <glm/gtc/type_ptr.hpp>

namespace Engine3D
{
    void BillboardObject::draw(Camera& cam)
    {
        if (m_texture.empty())
        {
            return;
        }
        
        glPushMatrix();

        glTranslatef(m_pos.x, m_pos.y, m_pos.z);

        glMultMatrixf(glm::value_ptr(cam.getRotationMatrix()));

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        glBegin(GL_QUADS);
        
        glTexCoord2f(1, 0);
        glVertex3f(m_scale.x * m_dims.x, -m_scale.y * m_dims.y, 0);
        glTexCoord2f(1, 1);
        glVertex3f(m_scale.x * m_dims.x, m_scale.y * m_dims.y, 0);
        glTexCoord2f(0, 1);
        glVertex3f(-m_scale.x * m_dims.x, m_scale.y * m_dims.y, 0);
        glTexCoord2f(0, 0);
        glVertex3f(-m_scale.x * m_dims.x, -m_scale.y * m_dims.y, 0);

        glEnd();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPopMatrix();
    }
};
