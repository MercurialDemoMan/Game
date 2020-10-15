#pragma once

#include "Macros.hpp"

#if ENGINE3D_PLATFORM == WINDOWS
#include <OpenGL/glew.h>
#else
#include <GL/glew.h>
#endif

#include "Canvas.hpp"
#include "Types.hpp"

namespace Engine3D
{
	/**
	 * GPU Frame Buffer Object
	 */
	class FBObject {
	public:
		
		/**
		 * constructors
		 *
		 * arguments specify width and height of the frame
		 */
		FBObject();
		FBObject(u32 width, u32 height);
		
		/**
		 * explicit initializer
		 */
		void init(u32 width, u32 height);
		
		/**
		 * free alocated framebuffers
		 */
		void clean();
		
		/**
		 * state that we intent to use this framebuffer
		 */
		void bind();
		/**
		 * state that we no longer want to use this framebuffer
		 */
		void unbind() const;
		
		/**
		 * get textures stored in GPU
		 */
		int getColorTexture() const { return m_color_texture; }
		int getDepthTexture() const { return m_depth_texture; }
		
		/**
		 * copy FBO content
		 */
		void resolveToFBO(FBObject& output_fbo) const;
		
		/**
		 * copy FBO content into the screen
		 */
		void resolveToScreen(u32 screen_width, u32 screen_height) const;
		
		/**
		 * draw FBO onto the screen
		 */
		void draw(u32 sub_texture = 0);
		
	private:

		void initFramebuffer();
		void createAndBindFramebuffer();
		void createTextureAttachment();
		void createDepthTextureAttachment();

		bool m_initialized { false };
		
		u32 m_width, m_height;
		GLuint m_framebuffer;
		GLuint m_color_texture;
		GLuint m_depth_texture;
		GLuint m_depth_buffer;
		GLuint m_color_buffer;

		Canvas m_canvas;
	};
};