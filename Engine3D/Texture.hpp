#pragma once

#include <string>

#include "Types.hpp"
#include "glm/glm.hpp"

namespace Engine3D
{
	/**
	 * GPU texture
	 */
	class Texture
	{
	public:
		/**
		 * constructors
		 */
		Texture(){}
		Texture(const char* id) { init(id); }
		Texture(const std::string& id) { init(id); }
		
		/**
		 * destructor
		 */
	    ~Texture() { clean(); }
		
		/**
		 * load texture into memory
		 */
		void init(const char* id) { init(std::string(id)); }
		void init(const std::string&& id);
		void init(const std::string& id);
		
		/**
		 * delete texture
		 */
		void clean();

		/**
		 * check if the texture is initialized
		 */
		bool empty();
		
		/**
		 * get texture ID from OpenGL
		 */
		u32 getID();

		/**
		 * get texture width and height
		 */
		glm::vec2 getDims();
		
	private:
	
		/**
		 * texture management
		 */
		std::string m_texture_id { "" };
	};
};