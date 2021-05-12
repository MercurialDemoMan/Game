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

#include "Macros.hpp"

#include <GL/glew.h>

#include "Canvas.hpp"
#include "Types.hpp"

namespace Engine3D
{
    /**
     * GPU Frame Buffer Object
     */
    class FBObject {
    public:
        
        /**
         * constructors
         *
         * arguments specify width and height of the frame
         */
        FBObject();
        FBObject(u32 width, u32 height);
        
        /**
         * explicit initializer
         */
        void init(u32 width, u32 height);
        
        /**
         * free alocated framebuffers
         */
        void clean();
        
        /**
         * state that we intent to use this framebuffer
         */
        void bind();
        /**
         * state that we no longer want to use this framebuffer
         */
        void unbind() const;
        
        /**
         * get textures stored in GPU
         */
        int getColorTexture() const { return m_color_texture; }
        int getDepthTexture() const { return m_depth_texture; }
        
        /**
         * copy FBO content
         */
        void resolveToFBO(FBObject& output_fbo) const;
        
        /**
         * copy FBO content into the screen
         */
        void resolveToScreen(u32 screen_width, u32 screen_height) const;
        
        /**
         * draw FBO onto the screen
         */
        void draw(u32 sub_texture = 0);
        
    private:

        void initFramebuffer();
        void createAndBindFramebuffer();
        void createTextureAttachment();
        void createDepthTextureAttachment();

        bool m_initialized { false };
        
        u32 m_width, m_height;
        GLuint m_framebuffer;
        GLuint m_color_texture;
        GLuint m_depth_texture;
        GLuint m_depth_buffer;
        GLuint m_color_buffer;

        Canvas m_canvas;
    };
};