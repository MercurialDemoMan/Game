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

namespace Engine3D
{
    namespace System
    {
        /**
         * initialize system module
         */
        void init(int argc, const char* argv[]);
        
        /**
         * get full path
         */
        std::string getFullPath(const std::string& path);
        std::string getFullPath(const char* path);
        
        /**
         * random number generator
         */
        float random();
        float random(float max);
        float random(float min, float max);
    };
};