#pragma once

#include <string>

#include "Macros.hpp"

#if ENGINE3D_PLATFORM == WINDOWS
#include <OpenGL/glew.h>
#include <SDL2/SDL_ttf.h>
#else
#include <GL/glew.h>
#include <SDL2/SDL_ttf.h>
#endif

#include <glm/glm.hpp>

#include "Game.hpp"
#include "Types.hpp"

namespace Engine3D
{
	/**
	 * 2D text manager
	 */
	class Text
	{
	public:
		
		/**
		 * constructors
		 */
		Text() {}
		Text(Engine3D::Game& game, const std::string& font_path, u32 font_size = DefaultFontSize);
	   ~Text();
		
		/**
		 * explicit constructor
		 */
		void init(Engine3D::Game& game, const std::string& font_path, u32 font_size = DefaultFontSize);
		
		/**
		 * clear
		 */
		void clear();

		/**
		 * update text texture
		 */
		void update(const std::string&& text);
		void update(const char* text);
		
		/**
		 * access attributes
		 */
		void       setColor(SDL_Color font_color) { m_font_color = font_color; }
		SDL_Color  getColor() const               { return m_font_color; }
		u32        getTexture() const             { return m_texture; }
		glm::vec2& pos()                          { return m_pos; }
		glm::vec2  getDims() const                { return glm::vec2(m_font_width, m_font_height); }

		/**
		 * draw text
		 */
		void draw();
		
		/**
		 * update text texture with "=" operator
		 */
		Text& operator=(const char* text);
		Text& operator=(const std::string& text) { *this = text.c_str(); return *this; }

		/**
		 * resize font, requires to reopen the font
		 */
		void resize(u32 size);

		static constexpr u32 DefaultFontSize = 50;
		
	private:
	
		glm::vec2   m_pos { 0 };

		glm::vec2   m_screen_dims;

		u32         m_texture { 0 };
		TTF_Font*   m_font;
		std::string m_font_buffer;
		u32         m_font_size   { DefaultFontSize };
		u32         m_font_width  { 200 };
		u32         m_font_height { 200 };
		SDL_Color   m_font_color { 0xFF, 0xFF, 0xFF, 0xFF };

		std::string m_font_path { "" };
	};
};