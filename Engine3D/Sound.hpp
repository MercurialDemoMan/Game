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

#include "Macros.hpp"

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>

#include "Cache.hpp"

namespace Engine3D
{
    /**
     * sound player
     */
    class Sound
    {
    public:
    
        /**
         * constructors
         */
        Sound() {}
        Sound(const char* sound_path);
        Sound(const std::string& sound_path);
        
        /**
         * destructor
         */
       ~Sound();
        
        /**
         * explicit constructors
         */
        void init(const char* sound_path);
        void init(const std::string& sound_path) { init(sound_path.c_str()); }
        
        /**
         * deallocate
         */
        void clean();
        
        /**
         * play the sound
         *
         * @arg loops - -1 infinite looping
         */
        void play(int loops = 0);
        
        /**
         * set sound volume
         */
        void volume(u8 value);
        
        /**
         * total number of allocated audio channels
         */
        constexpr static u32 NumAudioChannels = 16;
        
    private:
    
        float m_volume_scale { 1.0f };
    
        Mix_Chunk* m_sound { nullptr };
    };
};