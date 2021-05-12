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

#include "File.hpp"


namespace Engine3D
{
    /**
     * GPU cubemap
     */
    class Cubemap
    {
        public:
        /**
         * constructors
         */
        Cubemap(){}
        Cubemap(const char* id) { init(id); }
        Cubemap(const std::string& id) { init(id.c_str()); }
        
        /**
         * destructor
         */
        ~Cubemap() { clear(); }
        
        /**
         * load cubemap into memory
         */
        void init(const char* id) { init(std::string(id)); }
        void init(const std::string&& id);
        
        /**
         * delete cubemap
         */
        void clear();
        
        /**
         * get cubemap ID from OpenGL
         */
        u32 getID() const;
        
    private:
    
        /**
         * cubemap management
         */
        std::string m_cubemap_id { "" };
    };
};