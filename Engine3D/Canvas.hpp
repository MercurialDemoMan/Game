#pragma once

#include "Macros.hpp"

#include <GL/glew.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>

#include "Types.hpp"

namespace Engine3D
{
	/**
	 * 2D screen drawing plane
	 */
	class Canvas
	{
	public:
		
		/**
		 * constructor
		 */
		Canvas() {}
		
		/**
		 * draw the canvas onto the screen
		 */
		void draw();
		
		/**
		 * texture management
		 */
		void setMainTexture(u32 texture) { m_main_tex = texture; }
		void setSubTexture(u32 texture)  { m_sub_tex  = texture; }
				
	private:
		
		bool m_flip_x { false };
		bool m_flip_y { false };
		
		u32 m_main_tex { 0 };
		u32 m_sub_tex  { 0 };
	};
};