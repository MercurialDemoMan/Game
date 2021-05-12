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
#include "Sound.hpp"

#include "System.hpp"

namespace Engine3D
{
    /**
     * constructors
     */
    Sound::Sound(const char* sound_path)
    {
        init(sound_path);
    }
    Sound::Sound(const std::string& sound_path)
    {
        init(sound_path.c_str());
    }
    
    /**
     * destructor
     */
    Sound::~Sound()
    {
        clean();
    }
    
    /**
     * explicit constructors
     */
    void Sound::init(const char* sound_path)
    {
        m_sound = Mix_LoadWAV(System::getFullPath(sound_path).c_str());
        
        if(m_sound == nullptr)
        {
            //TODO: warning
        }
    }
    
    /**
     * deallocate
     */
    void Sound::clean()
    {
        if(m_sound != nullptr)
        {
            Mix_FreeChunk(m_sound);
            m_sound = nullptr;
        }
    }
    
    /**
     * play the music
     *
     * @arg loops - -1 infinite looping
     */
    void Sound::play(int loops/* = 1*/)
    {
        if(m_sound == nullptr)
        {
            //TODO: warning
            return;
        }
        
        if(Mix_PlayChannel(-1, m_sound, loops) == -1)
        {
            if(Mix_PlayChannel(0, m_sound, loops) == -1)
            {
                //TODO: warning
            }
        }
    }
    
    /**
     * pause the music
     */
    void Sound::volume(u8 value)
    {
        if(m_sound == nullptr)
        {
            //TODO: warning
            return;
        }
        
        m_sound->volume = static_cast<u8>(value * m_volume_scale);
    }
};