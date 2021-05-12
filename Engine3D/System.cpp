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
#include "System.hpp"

#include <random>

namespace Engine3D
{
    namespace System
    {
        std::string m_executable_path;   // full executable path
        
        /**
         * initialize system module
         */
        void init(int argc, const char* argv[])
        {
            // get executable folder location
            m_executable_path = std::string(argv[0]);
            
            std::size_t slash_index = m_executable_path.find_last_of("/\\");
            m_executable_path       = m_executable_path.substr(0, slash_index);
            
            #ifdef _WIN32
                m_executable_path += '\\';
            #else
                m_executable_path += '/';
            #endif
        }
        
        /**
         * get full path
         */
        std::string getFullPath(const std::string& path)
        {
            return m_executable_path + path;
        }
        std::string getFullPath(const char* path)
        {
            return m_executable_path + path;
        }
    };
};