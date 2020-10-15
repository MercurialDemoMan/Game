#pragma once

#include <glm/glm.hpp>

#include "Mesh.hpp"
#include "Texture.hpp"

namespace Engine3D
{
	/**
	 * 3D object with mesh
	 */
	class SceneObject
	{
	public:
	
		/**
		 * constructors
		 */
		SceneObject() {}
		SceneObject(glm::vec3 pos) : m_pos(pos) {}
		SceneObject(glm::vec3 pos, const char* mesh_id) : m_pos(pos)
		{
			m_mesh.init(mesh_id);
		}
		SceneObject(glm::vec3 pos, const std::string& mesh_id) : m_pos(pos)
		{
			m_mesh.init(mesh_id);
		}
		
		/**
		 * explicit initializers
		 */
		void init(glm::vec3 pos) { m_pos = pos; }
		void init(glm::vec3 pos, const char* mesh_id) { m_pos = pos; m_mesh.init(mesh_id); }
		void init(glm::vec3 pos, const std::string& mesh_id) { m_pos = pos; m_mesh.init(mesh_id); }
		
		/**
		 * access attributes
		 */
		glm::vec3& pos()            { return m_pos; }
		glm::vec3& rot()            { return m_rot; }
		glm::vec3& dims()           { return m_dims; }
		glm::vec3& scale()          { return m_scale; }
		const Material* material()  { return m_mesh.material(); }
		
		const std::vector<Engine3D::Vertex>* vertices() { return m_mesh.vertices(); }

		/**
		 * deletes the data saved in ram
		 */
		void discardVertices() { m_mesh.discardVertices(); }

		/**
		 * extract triangle from vertices
		 */
		Triangle constructTriangle(u32 vert_index_start);

		/**
		 * draw 3D object
		 */
		void draw();
	
	protected:
	
		glm::vec3 m_pos     { 0 };
		glm::vec3 m_rot     { 0 };
		glm::vec3 m_dims    { 1, 1, 1 };
		glm::vec3 m_scale   { 1, 1, 1 };
		Texture   m_texture;

		Engine3D::Mesh m_mesh;
	};
};