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
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>

#include "Types.hpp"

namespace Engine3D
{
    /**
     * 2D screen drawing plane
     */
    class Canvas
    {
    public:
        
        /**
         * constructor
         */
        Canvas() {}
        
        /**
         * draw the canvas onto the screen
         */
        void draw();
        
        /**
         * texture management
         */
        void setMainTexture(u32 texture) { m_main_tex = texture; }
        void setSubTexture(u32 texture)  { m_sub_tex  = texture; }
                
    private:
        
        bool m_flip_x { false };
        bool m_flip_y { false };
        
        u32 m_main_tex { 0 };
        u32 m_sub_tex  { 0 };
    };
};