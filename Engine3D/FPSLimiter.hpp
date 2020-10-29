#pragma once

#include <SDL2/SDL.h>

#include "Types.hpp"

namespace Engine3D
{
    /**
     * utility for limiting CPU clock
     */
    class FPSLimiter
    {
    public:
        
        /**
         * constructors
         */
        FPSLimiter();
        FPSLimiter(float max_fps);

        /**
         * update maximum frames per second
         */
        void  setMaxFPS(float max_fps);
        float getMaxFPS() { return m_max_fps; }

        /**
         * start processing frame
         *
         * \note this should go at the start of the frame
         */
        void  begin();
        
        /**
         * end processing frame and limit the CPU clock
         *
         * \return calculated fps
         */
        float end();

        /**
         * toggle limiting fps, but not actually calculating them
         */
        bool& enabled() { return m_enabled; }
        
    private:
        
        void calculateFPS();

        float m_max_fps { 60.0f };
        float m_fps;
        float m_frame_time;
        u32   m_start_ticks;
        bool  m_enabled { true };
    };
};