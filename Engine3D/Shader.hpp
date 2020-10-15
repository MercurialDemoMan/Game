#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "Types.hpp"
#include "Cache.hpp"
#include "File.hpp"

namespace Engine3D
{
	class ShaderSource
	{
	public:

		ShaderSource(const char* source) : m_source(source)
		{

		}

		const char* source() const { return m_source; }

	private:

		const char* m_source { nullptr };
	};

	/**
	 * GPU Shader
	 */
	class Shader
	{
	public:
	
		/**
		 * constructor
		 */
		Shader() {}
		Shader(const char* vertex_shader_path, const char* fragment_shader_path);
		Shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
		Shader(ShaderSource vertex_shader, ShaderSource fragment_shader);
		
		/**
		 * destructor
		 */
	   ~Shader();

		/**
		 * state that we wanto to use this program
		 */
		void use();
		
		/**
		 * stop using this program
		 */
		void unuse();

		/**
		 * compile shader program
		 */
		void init(const char* vertex_shader_path, const char* fragment_shader_path);
		void init(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
		{
		   init(vertex_shader_path.c_str(), fragment_shader_path.c_str());
		}

		/**
		 * setting uniforms in shaders
		 */
		void setTexture2D(u32 texture_id, const char* uniform_name);
		void setTexture3D(u32 texture_id, const char* uniform_name);
		void setTextureCubemap(u32 texture_id, const char* uniform_name);
		void set1i(int         value, const char* uniform_name);
		void set1f(float       value, const char* uniform_name);
		void set2f(glm::vec2   value, const char* uniform_name);
		void set3f(glm::vec3   value, const char* uniform_name);
		void set4f(glm::vec4   value, const char* uniform_name);
		void set1b(bool        value, const char* uniform_name);
		void set4x4m(glm::mat4 value, const char* uniform_name);
		void set1fv(float*     array, unsigned size, const char* uniform_name);
		void set1iv(int*       array, unsigned size, const char* uniform_name);

		u32 getAttributeIndex(const char* attribute_name) const;
		
	private:
	
		std::string m_fragment_id;
		std::string m_vertex_id;
		int m_program { -1 };

		u32 m_free_texture { 0 };
	};
};