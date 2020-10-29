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