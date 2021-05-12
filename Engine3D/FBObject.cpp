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
#include "FBObject.hpp"

namespace Engine3D
{
    FBObject::FBObject()
    {
    }

    /**
     * constructors
     *
     * arguments specify width and height of the frame
     */
    FBObject::FBObject(u32 width, u32 height)
    {
        if(!m_initialized)
        {    
            init(width, height);
        }
        else
        {
            clean();
            m_initialized = false;
            init(width, height);
        }
    }

    /**
     * explicit initializer
     */
    void FBObject::init(u32 width, u32 height)
    {
        m_width  = width;
        m_height = height;

        if(m_initialized)
        {
            clean();
        }

        initFramebuffer();
        m_canvas = Canvas();
        m_canvas.setMainTexture(getColorTexture());
        
        m_initialized = true;
    }

    /**
     * free alocated framebuffers
     */
    void FBObject::clean()
    {
        glDeleteFramebuffers (1, &m_framebuffer);
        glDeleteTextures     (1, &m_color_texture);
        glDeleteTextures     (1, &m_depth_texture);
        glDeleteRenderbuffers(1, &m_depth_buffer);
        glDeleteRenderbuffers(1, &m_color_buffer);
        m_initialized = false;
    }

    /**
     * state that we intent to use this framebuffer
     */
    void FBObject::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
        glEnable         (GL_DEPTH_TEST);
        glClearDepth     (1.0);
        glClear          (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    /**
     * state that we no longer want to use this framebuffer
     */
    void FBObject::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    /**
     * copy FBO content
     */
    void FBObject::resolveToFBO(FBObject& output_fbo) const
    {
        // set source and destination
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, output_fbo.m_framebuffer);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, this->m_framebuffer);
        // copy framebuffer content
        glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, output_fbo.m_width, output_fbo.m_height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        // unbind buffers
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }
    
    /**
     * copy FBO content into the screen
     */
    void FBObject::resolveToScreen(u32 screen_width, u32 screen_height) const
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebuffer);
        glDrawBuffer(GL_BACK);
        glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, screen_width, screen_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        unbind();
    }

    /**
     * draw FBO onto the screen
     */
    void FBObject::draw(u32 sub_texture/* = 0*/)
    {
        m_canvas.setSubTexture(sub_texture);
        m_canvas.draw();
    }

    void FBObject::initFramebuffer()
    {
        createAndBindFramebuffer();
        createTextureAttachment();
        createDepthTextureAttachment();
        unbind();
    }

    void FBObject::createAndBindFramebuffer()
    {
        glGenFramebuffers(1, &m_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
    }

    void FBObject::createTextureAttachment()
    {
        glGenTextures(1, &m_color_texture);
        glBindTexture(GL_TEXTURE_2D, m_color_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_texture, 0);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void FBObject::createDepthTextureAttachment()
    {
        glGenTextures(1, &m_depth_texture);
        glBindTexture(GL_TEXTURE_2D, m_depth_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_texture, 0);
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};