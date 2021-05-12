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

        static constexpr u32 NumSamples = 10;
        
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

        float m_max_fps;
        float m_fps;
        float m_frame_time;
        u32   m_start_ticks;
        bool  m_enabled { true };
    };
};