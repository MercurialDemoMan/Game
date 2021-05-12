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

#include <string>

#include "Types.hpp"
#include "glm/glm.hpp"

namespace Engine3D
{
    /**
     * GPU texture
     */
    class Texture
    {
    public:
        /**
         * constructors
         */
        Texture(){}
        Texture(const char* id) { init(id); }
        Texture(const std::string& id) { init(id); }
        
        /**
         * destructor
         */
        ~Texture() { clean(); }
        
        /**
         * load texture into memory
         */
        void init(const char* id);
        void init(const std::string& id) { init(id.c_str()); }
        
        /**
         * delete texture
         */
        void clean();

        /**
         * check if the texture is initialized
         */
        bool empty();
        
        /**
         * get texture ID from OpenGL
         */
        u32 getID();

        /**
         * get texture width and height
         */
        glm::vec2 getDims();
        
    private:
    
        /**
         * texture management
         */
        std::string m_texture_id { "" };
    };
};