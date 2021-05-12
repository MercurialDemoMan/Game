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