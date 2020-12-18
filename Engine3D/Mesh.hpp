#pragma once

#include <vector>
#include <optional>

#include <glm/glm.hpp>

#include "Cache.hpp"
#include "Texture.hpp"
#include "Shapes.hpp"
#include "Shader.hpp"

namespace Engine3D
{
    static constexpr Engine3D::u32 DivisionFactor = 2;

    /**
     * vertex structure
     */
    struct Vertex
    {
        Vertex() {}
        Vertex(const glm::vec3& p_pos) : pos(p_pos) {}
        Vertex(const glm::vec3& p_pos, const glm::vec3& p_nor) : pos(p_pos), nor(p_nor) {}
        Vertex(const glm::vec3& p_pos, const glm::vec3& p_nor, const glm::vec2& p_uv) : pos(p_pos), nor(p_nor), uv(p_uv) {}

        glm::vec3 pos { 0 };
        glm::vec3 nor { 1, 0, 0 };
        glm::vec2 uv  { 0, 0 };
    };

    /**
     * material
     */
    struct Material
    {
        Material() {}
        Material(const glm::vec3& p_diffuse, const glm::vec3& p_ambient, const glm::vec3& p_specular, const float& p_shininess) : diffuse(p_diffuse), ambient(p_ambient), specular(p_specular)
        {

        }

        glm::vec3 diffuse;
        glm::vec3 ambient;
        glm::vec3 specular;
        Texture   diffuse_mapping_texture;
    };

    /**
     * GPU + CPU vertices
     */
    struct Vertices
    {
        u32 size;
        u32 vao;
        u32 vbo;
        Material material;
        bool     has_material;
        std::vector<Vertex> data;
        std::vector<std::vector<u32>> partitions;
        double   furthest_vertex_value;
    };

    /**
     * collection of vertices
     */
    class Mesh
    {
    public:

        /**
         * constructors
         */
        Mesh(){}
        Mesh(const char* id) { init(id); }
        Mesh(const std::string& id) { init(id); }
        /**
         * destructor
         */
       ~Mesh();
        
        /**
         * load verties into memory
         */
        void init(const char* id) { init(std::string(id)); }
        void init(const std::string&& id);
        void init(const std::string& id);
        
        /**
         * check if mesh has any vertices or 
         * if it hasn't been initialized
         */
        bool empty() const;
        
        /**
         * draw mesh
         */
        void draw();
        
        /**
         * deallocate vertices
         */
        void clear();

        /**
         * get vertices
         */
        const std::vector<Vertex>* vertices();

        /**
         * deletes the data saved in ram
         */
        void discardVertices();

        /**
         * get material
         */
        const Material* material();

        /**
         * extract triangle from vertices
         */
        Triangle constructTriangle(u32 vert_index_start);


        void bindVertexPositionWithShader(const Shader& shader, const char* attribute_name);
        void bindVertexNormalWithShader(const Shader& shader, const char* attribute_name);
        void bindVertexUVWithShader(const Shader& shader, const char* attribute_name);

        /**
         * get raw vertices
         */
        const Vertices* rawVertices();
        
    private:

        /**
         * vertices management
         */
        std::string m_vertices_id { "" };
    };
};