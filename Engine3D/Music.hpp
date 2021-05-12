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

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "Cache.hpp"

namespace Engine3D
{
    /**
     * music player
     */
    class Music
    {
    public:
    
        /**
         * constructors
         */
        Music() {}
        Music(const char* music_path);
        Music(const std::string& music_path);
        
        /**
         * destructor
         */
       ~Music();
        
        /**
         * explicit constructors
         */
        void init(const char* music_path);
        void init(const std::string& music_path) { init(music_path.c_str()); }
        
        /**
         * deallocate
         */
        void clean();
        
        /**
         * play the music
         *
         * @arg loops - -1 infinite looping
         */
        void play(int loops = 1);
        
        /**
         * pause the music
         */
        void pause();
        
        /**
         * stop the music
         */
        void stop();
        
        /**
         * resume the music
         */
        void resume();
        
        
    private:
    
        Mix_Music* m_music { nullptr };
    };
};