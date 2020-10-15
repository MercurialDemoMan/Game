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
	float FPSLimiter::end() {
		
		calculateFPS();

		float frame_ticks = static_cast<float>(SDL_GetTicks() - m_start_ticks);

		if (m_enabled)
		{
			if (1000.0f / m_max_fps > frame_ticks)
			{
				SDL_Delay(static_cast<u32>(1000.0f / m_max_fps - frame_ticks));
			}
		}

		return m_fps;
	}

	void FPSLimiter::calculateFPS()
	{
		static constexpr u32 NumSamples = 10;
		static float frame_times[NumSamples];
		static int   current_frame = 0;
		static float prev_ticks = static_cast<float>(SDL_GetTicks());
		
		float current_ticks = static_cast<float>(SDL_GetTicks());

		m_frame_time = current_ticks - prev_ticks;
		frame_times[current_frame++ % NumSamples] = m_frame_time;

		prev_ticks = current_ticks;

		int count;
		if (current_frame < NumSamples) { count = current_frame; }
		else 							{ count = NumSamples;    }

		float frame_time_average = 0;
		
		for (int i = 0; i < count; i++) 
		{
			frame_time_average += frame_times[i];
		}
		frame_time_average /= count;

		if (frame_time_average > 0) 
		{
			m_fps = 1000.0f / frame_time_average;
		}
		else 
		{
			m_fps = 60.0f;
		}
	}
};