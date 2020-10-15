#include "Music.hpp"

#include "System.hpp"

namespace Engine3D
{
	/**
	 * constructors
	 */
	Music::Music(const char* music_path)
	{
		init(music_path);
	}
	Music::Music(const std::string& music_path)
	{
		init(music_path.c_str());
	}
	
	/**
	 * destructor
	 */
	Music::~Music()
	{
		clean();
	}
	
	/**
	 * explicit constructors
	 */
	void Music::init(const char* music_path)
	{
		m_music = Mix_LoadMUS(System::getFullPath(music_path).c_str());
		
		if(m_music == nullptr)
		{
			//TODO: warning
		}
	}
	
	/**
	 * deallocate
	 */
	void Music::clean()
	{
		if(m_music != nullptr)
		{
			stop();
			Mix_FreeMusic(m_music);
			m_music = nullptr;
		}
	}
	
	/**
	 * play the music
	 *
	 * @arg loops - -1 infinite looping
	 */
	void Music::play(int loops/* = 1*/)
	{
		if(m_music == nullptr)
		{
			//TODO: warning
			return;
		}
		
		if(Mix_PlayMusic(m_music, loops) == -1)
		{
			//TODO: warning
		}
	}
	
	/**
	 * pause the music
	 */
	void Music::pause()
	{
		if(m_music == nullptr)
		{
			//TODO: warning
			return;
		}
		
		Mix_PauseMusic();
	}
	
	/**
	 * stop the music
	 */
	void Music::stop()
	{
		if(m_music == nullptr)
		{
			//TODO: warning
			return;
		}
		
		Mix_HaltMusic();
	}
	
	/**
	 * resume the music
	 */
	void Music::resume()
	{
		if(m_music == nullptr)
		{
			//TODO: warning
			return;
		}
		
		Mix_ResumeMusic();
	}
};