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
#include "FPSLimiter.hpp"

#include <cstdio>

namespace Engine3D
{
    /**
     * constructors
     */
    FPSLimiter::FPSLimiter() 
    {
    }
    FPSLimiter::FPSLimiter(float max_fps) :
        m_max_fps(max_fps)
    {
    }
    
    /**
     * update maximum frames per second
     */
    void FPSLimiter::setMaxFPS(float max_fps)
    {
        m_max_fps = max_fps;
    }

    /**
     * start processing frame
     *
     * \note this should go at the start of the frame
     */
    void FPSLimiter::begin()
    {
        m_start_ticks = SDL_GetTicks();
    }

    /**
     * end processing frame and limit the CPU clock
     *
     * \return calculated fps
     */
    float FPSLimiter::end() 
    {
        float frame_ticks = static_cast<float>(SDL_GetTicks() - m_start_ticks);

        if (frame_ticks == 0)
        {
            return m_max_fps;
        }

        if (m_enabled)
        {
            if (1000.0f / m_max_fps > frame_ticks)
            {
                SDL_Delay(static_cast<u32>((1000.0f / m_max_fps) - frame_ticks));
            }
        }

        m_fps = 1000.0f / static_cast<float>(SDL_GetTicks() - m_start_ticks);;

        return m_fps;
    }
};