#include "System.hpp"

#include <random>

namespace Engine3D
{
    namespace System
    {
        std::string m_executable_path;   // full executable path
        
        std::random_device m_random_dev; // random device
        std::mt19937       m_random_gen; // random generator
        
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
            
            // initialize random number generator
            m_random_gen = std::mt19937(m_random_dev());
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
        
        /**
         * random number generator
         */
        float random() 
        {
            static std::uniform_real_distribution<float> random_dist(0.0f, 1.0f);
            
            return random_dist(m_random_gen);
        }
        float random(float max)
        {
            std::uniform_real_distribution<float> random_dist;
            
            if(max < 0.0f)
            {
                random_dist = std::uniform_real_distribution<float>(max, 0.0f);
            }
            else
            {
                random_dist = std::uniform_real_distribution<float>(0.0, max);
            }
            
            return random_dist(m_random_gen);
        }
        float random(float min, float max)
        {
            std::uniform_real_distribution<float> random_dist;
            
            if(min < max)
            {
                random_dist = std::uniform_real_distribution<float>(min, max);
            }
            else
            {
                random_dist = std::uniform_real_distribution<float>(max, min);
            }
            
            return random_dist(m_random_gen);
        }
    };
};