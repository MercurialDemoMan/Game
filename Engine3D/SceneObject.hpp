#pragma once

#include <optional>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Mesh.hpp"
#include "Texture.hpp"
#include "Shapes.hpp"

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
        glm::mat4& rot()            { return m_rot; }
        glm::mat4& rotate(float angle, glm::vec3 normal)
        {
        	glm::quat q = 
        	{
        		cos(angle / 2.0f),
        		normal.x * sin(angle / 2.0f),
        		normal.y * sin(angle / 2.0f),
        		normal.z * sin(angle / 2.0f)
        	};

        	m_rot = m_rot * glm::toMat4(q);

        	return m_rot;
        }
        glm::mat4& set_rot(float angle, glm::vec3 normal)
        {
        	glm::quat q = 
        	{
        		cos(angle / 2.0f),
        		normal.x * sin(angle / 2.0f),
        		normal.y * sin(angle / 2.0f),
        		normal.z * sin(angle / 2.0f)
        	};

        	m_rot = glm::toMat4(q);

        	return m_rot;
        }
        glm::vec3& dims()           { return m_dims; }
        glm::vec3& scale()          { return m_scale; }
        const Material* material()  { return m_mesh.material(); }
        
        const std::vector<Engine3D::Vertex>* vertices() { return m_mesh.vertices(); }

        /**
         * get bounding box
         */
        Engine3D::Box boundingBox() 
        { 
        	return Engine3D::Box
        	(
        		m_pos, 
        		m_scale * glm::vec3(2.0f * m_mesh.rawVertices()->furthest_vertex_value)
        	); 
        }

        Engine3D::Box getDivisionBox(u32 x, u32 y, u32 z)
        {
            glm::vec3 translated_pos = (glm::vec3(x, y, z) / float(DivisionFactor)) + (1.0f / DivisionFactor) / 2.0f;
            return Engine3D::Box(m_pos + translated_pos, glm::vec3(1.0f / DivisionFactor));
        }

        const std::vector<u32>& getDivision(u32 x, u32 y, u32 z)
        {
            if (x >= DivisionFactor || y >= DivisionFactor || z >= DivisionFactor)
            {
                return { };
            }

            return m_mesh.rawVertices()->partitions[x + DivisionFactor * (y + DivisionFactor * z)];
        }

        /**
         * deletes the data saved in ram
         */
        void discardVertices() { m_mesh.discardVertices(); }

        /**
         * extract vertex
         */
        std::optional<Vertex> getVertex(u32 i);

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
        glm::mat4 m_rot     { 1 };
        glm::vec3 m_dims    { 1, 1, 1 };
        glm::vec3 m_scale   { 1, 1, 1 };
        Texture   m_texture;

        Engine3D::Mesh m_mesh;
    };
};