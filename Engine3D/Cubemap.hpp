#pragma once

#include <string>

#include "File.hpp"


namespace Engine3D
{
	/**
	 * GPU cubemap
	 */
	class Cubemap
	{
		public:
		/**
		 * constructors
		 */
		Cubemap(){}
		Cubemap(const char* id) { init(id); }
		Cubemap(const std::string& id) { init(id.c_str()); }
		
		/**
		 * destructor
		 */
	    ~Cubemap() { clear(); }
		
		/**
		 * load cubemap into memory
		 */
		void init(const char* id) { init(std::string(id)); }
		void init(const std::string&& id);
		
		/**
		 * delete cubemap
		 */
		void clear();
		
		/**
		 * get cubemap ID from OpenGL
		 */
		u32 getID() const;
		
	private:
	
		/**
		 * cubemap management
		 */
		std::string m_cubemap_id { "" };
	};
};