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