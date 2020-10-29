#include "Canvas.hpp"

namespace Engine3D
{
    
    void Canvas::draw()
    {
        bool was_enabled_depth_test = glIsEnabled(GL_DEPTH_TEST);

        glDisable(GL_DEPTH_TEST);
        
        if(m_main_tex != 0) 
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_main_tex);
        }
        
        if(m_sub_tex != 0) 
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_sub_tex);
        }
        
        glBegin(GL_QUADS);
        glTexCoord2f(m_flip_x, !m_flip_y);
        glVertex3f(1, 1, 0);
        glTexCoord2f(!m_flip_x, !m_flip_y);
        glVertex3f(-1, 1, 0);
        glTexCoord2f(!m_flip_x, m_flip_y);
        glVertex3f(-1, -1, 0);
        glTexCoord2f(m_flip_x, m_flip_y);
        glVertex3f(1, -1, 0);
        glEnd();
        
        glBindTexture(GL_TEXTURE_2D, 0);
        
        if (was_enabled_depth_test)
        {
            glEnable(GL_DEPTH_TEST);
        }
    }
}