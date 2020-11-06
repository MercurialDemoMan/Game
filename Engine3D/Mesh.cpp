#include "Mesh.hpp"

#include <unordered_map>
#include <limits>
#include <cstdio>
#include <stdexcept>

#include "Macros.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>



namespace Engine3D
{
    /**
     * load vertices from file
     */
    Vertices meshCacheLoadingFunction(File& input_file)
    {
        std::printf("Mesh() log: loading file: %s\n", input_file.getPath().c_str());

        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<Vertex>    vertices;
        std::vector<glm::vec2> uv_mapping;

        Material               material;
        bool                   has_material = false;

        char material_file_buffer[1024];
        
        glm::vec3 container;
        u32 fa,  fb,  fc,  fd;
        u32 fna, fnb, fnc, fnd;
        u32 fta, ftb, ftc, ftd;

        auto normalize_mesh = [](std::vector<Vertex>& vertices)
        {
            if (vertices.size() == 0)
                return;

            glm::vec3 center;

            glm::vec3 mesh_max = glm::vec3(-std::numeric_limits<float>::infinity());
            glm::vec3 mesh_min = glm::vec3(std::numeric_limits<float>::infinity());
            //find the center of the model
            for (auto& vertex : vertices)
            {
                if (vertex.pos.x > mesh_max.x) { mesh_max.x = vertex.pos.x; }
                if (vertex.pos.y > mesh_max.y) { mesh_max.y = vertex.pos.y; }
                if (vertex.pos.z > mesh_max.z) { mesh_max.z = vertex.pos.z; }

                if (vertex.pos.x < mesh_min.x) { mesh_min.x = vertex.pos.x; }
                if (vertex.pos.y < mesh_min.y) { mesh_min.y = vertex.pos.y; }
                if (vertex.pos.z < mesh_min.z) { mesh_min.z = vertex.pos.z; }
            }
            center = mesh_min + (mesh_max - mesh_min) / 2.0f;

            //move the whole model to 0, 0, 0 and find the furthest vertex from the center
            u32   max_index = 0;
            float max_value = 0;
            for (size_t i = 0; i < vertices.size(); i++)
            {
                vertices[i].pos -= center;

                if (fabs(vertices[i].pos.x) > max_value) { max_value = fabs(vertices[i].pos.x); max_index = i; continue; }
                if (fabs(vertices[i].pos.y) > max_value) { max_value = fabs(vertices[i].pos.y); max_index = i; continue; }
                if (fabs(vertices[i].pos.z) > max_value) { max_value = fabs(vertices[i].pos.z); max_index = i; continue; }
            }


            //check for bullshit
            if (max_value == 0)
            {
                return;
            }

            float size_delta = (0.5f / max_value);

            for (auto& vertex : vertices)
            {
                vertex.pos = vertex.pos * size_delta;
            }
        };

        auto parse_material = [&](File& input_file)
        {
            std::vector<u8> buffer = input_file.read();
            std::string     line;

            glm::vec3 container;

            char diffuse_map_file[1024];

            for (u32 i = 0; i < buffer.size(); i++)
            {
                if(buffer[i] == '\n')
                {
                    if (line.size() > 2)
                    {
                        if (line[0] == 'K')
                        {
                            if(line[1] == 'a')
                            {
                                if (std::sscanf(reinterpret_cast<char*>(&line[0]), "Ka %f %f %f", &container.x, &container.y, &container.z) == 3)
                                {
                                    has_material = true;
                                    material.ambient = container;
                                }
                            }
                            else if(line[1] == 'd')
                            {
                                if (std::sscanf(reinterpret_cast<char*>(&line[0]), "Kd %f %f %f", &container.x, &container.y, &container.z) == 3)
                                {
                                    has_material = true;
                                    material.diffuse = container;
                                }
                            }
                            else if(line[1] == 's')
                            {
                                if (std::sscanf(reinterpret_cast<char*>(&line[0]), "Ks %f %f %f", &container.x, &container.y, &container.z) == 3)
                                {
                                    has_material = true;
                                    material.specular = container;
                                }
                            }
                        }
                        else if (line[0] == 'm')
                        {
                            if (std::sscanf(reinterpret_cast<char*>(&line[0]), "map_Kd %s1023", diffuse_map_file) == 1)
                            {
                                has_material = true;
                                //TODO: this shouldnt be hard string
                                material.diffuse_mapping_texture.init(std::string("data/objects/") + diffuse_map_file);
                            }
                        }
                    }
                    line.clear();
                }
                else
                {
                    line += buffer[i];
                }
            }
        };
        
        std::vector<u8> buffer = input_file.read();
        std::string     line;

        u32 faces = 0;
        
        for(u32 i = 0; i < buffer.size(); i++)
        {
            if(buffer[i] == '\n')
            {
                if (line.size() > 0)
                {
                    // parse vertex
                    if(line[0] == 'v')
                    {
                        // parse vertex position
                        if(line[1] == ' ')
                        {
                            if(std::sscanf(reinterpret_cast<char*>(&line[0]), "v %f %f %f", &container.x, &container.y, &container.z) == 3)
                            {
                                positions.push_back(container);
                            }
                            else
                            {
                                std::printf("Mesh::cacheLoadingFunction() warning: unknown data: %s\n", line.c_str());
                            }
                        }
                        // parse vertex normal
                        else if(line[1] == 'n')
                        {
                            if(std::sscanf(reinterpret_cast<char*>(&line[0]), "vn %f %f %f", &container.x, &container.y, &container.z) == 3)
                            {
                                normals.push_back(container);
                            }
                            else
                            {
                                std::printf("Mesh::cacheLoadingFunction() warning: unknown data: %s\n", line.c_str());
                            }
                        }
                        // parse vertex uv mapping
                        else if (line[1] == 't')
                        {
                            if (std::sscanf(reinterpret_cast<char*>(&line[0]), "vt %f %f", &container.x, &container.y) == 2)
                            {
                                uv_mapping.emplace_back(container.x, container.y);
                            }
                            else
                            {
                                std::printf("Mesh::cacheLoadingFunction() warning: unknown data: %s\n", line.c_str());
                            }
                        }
                    }
                    // parse face
                    else if(line[0] == 'f')
                    {
                        // parse triangles
                        if(std::count(line.begin(), line.end(), ' ') == 3)
                        {
                            if(std::sscanf(reinterpret_cast<char*>(&line[0]), "f %u//%u %u//%u %u//%u", &fa, &fna, &fb, &fnb, &fc, &fnc) == 6)
                            {
                                vertices.emplace_back(positions[fa - 1], normals[fna - 1]);
                                vertices.emplace_back(positions[fb - 1], normals[fnb - 1]);
                                vertices.emplace_back(positions[fc - 1], normals[fnc - 1]);

                                faces++;
                            }
                            else
                            {
                                if (std::sscanf(reinterpret_cast<char*>(&line[0]), "f %u/%u/%u %u/%u/%u %u/%u/%u", &fa, &fta, &fna, &fb, &ftb, &fnb, &fc, &ftc, &fnc) == 9)
                                {
                                    vertices.emplace_back(positions[fa - 1], normals[fna - 1], uv_mapping[fta - 1]);
                                    vertices.emplace_back(positions[fb - 1], normals[fnb - 1], uv_mapping[ftb - 1]);
                                    vertices.emplace_back(positions[fc - 1], normals[fnc - 1], uv_mapping[ftc - 1]);

                                    faces++;
                                }
                                else
                                {
                                    std::printf("Mesh::cacheLoadingFunction() warning: unknown data: %s\n", line.c_str());
                                    //TODO: warning?
                                }
                            }
                        }
                        // parse quads
                        else
                        {
                            if(std::sscanf(reinterpret_cast<char*>(&line[0]), "f %u//%u %u//%u %u//%u %u//%u", &fa, &fna, &fb, &fnb, &fc, &fnc, &fd, &fnd) == 8)
                            {
                                vertices.emplace_back(positions[fa - 1], normals[fna - 1]);
                                vertices.emplace_back(positions[fb - 1], normals[fnb - 1]);
                                vertices.emplace_back(positions[fc - 1], normals[fnc - 1]);
                                    
                                vertices.emplace_back(positions[fa - 1], normals[fna - 1]);
                                vertices.emplace_back(positions[fc - 1], normals[fnc - 1]);
                                vertices.emplace_back(positions[fd - 1], normals[fnd - 1]);

                                faces++; faces++;
                            }
                            else
                            {
                                if (std::sscanf(reinterpret_cast<char*>(&line[0]), "f %u/%u/%u %u/%u/%u %u/%u/%u %u/%u/%u", &fa, &fta, &fna, &fb, &ftb, &fnb, &fc, &ftc, &fnc, &fd, &ftd, &fnd) == 12)
                                {
                                    vertices.emplace_back(positions[fa - 1], normals[fna - 1], uv_mapping[fta - 1]);
                                    vertices.emplace_back(positions[fb - 1], normals[fnb - 1], uv_mapping[ftb - 1]);
                                    vertices.emplace_back(positions[fc - 1], normals[fnc - 1], uv_mapping[ftc - 1]);

                                    vertices.emplace_back(positions[fa - 1], normals[fna - 1], uv_mapping[fta - 1]);
                                    vertices.emplace_back(positions[fc - 1], normals[fnc - 1], uv_mapping[ftc - 1]);
                                    vertices.emplace_back(positions[fd - 1], normals[fnd - 1], uv_mapping[ftd - 1]);

                                    faces++; faces++;
                                }
                                else
                                {
                                    std::printf("Mesh::cacheLoadingFunction() warning: unknown data: %s\n", line.c_str());
                                }
                            }
                        }
                    }
                    // skip comment and nonsence sized lines
                    else if (line[0] == '#' || line.size() < 3)
                    {
                        (void)0;
                    }
                    // parse material file
                    else if (line[0] == 'm')
                    {
                        if(std::sscanf(reinterpret_cast<char*>(&line[0]), "mtllib %1023s", material_file_buffer) == 1)
                        {
                            File material(input_file.getFolder() + "/" + material_file_buffer);

                            if(material.opened())
                            {
                                parse_material(material);
                                material.close();
                            }
                            else
                            {
                                std::printf("Mesh::cacheLoadingFunction() warning: file cannot be opened: %s\n", material_file_buffer);
                            }
                        }
                        else
                        {
                            std::printf("Mesh::cacheLoadingFunction() warning: unknown data: %s\n", line.c_str());
                        }
                    }
                }

                line.clear();
            }
            else
            {
                line += buffer[i];
            }
        }

        if (vertices.size() == 0)
        {
            throw std::runtime_error("Mesh::load() error: no data in .obj file");
        }

        normalize_mesh(vertices);

        // create vao and vbo
        Vertices result;
        result.size = vertices.size();
        
        glGenVertexArrays(1, &result.vao);
        glGenBuffers(1, &result.vbo);
        glBindVertexArray(result.vao);

        glBindBuffer(GL_ARRAY_BUFFER, result.vbo);

        // copy vertex data
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        result.data         = std::move(vertices);
        result.has_material = has_material;
        if(result.has_material)
        { 
            result.material = std::move(material);
        }
        
        std::printf("Mesh() log: %s - loaded %u vertices\n", input_file.getPath().c_str(), result.data.size());

        return result;
    }
    
    /**
     * clear vertices
     */
    void meshCacheClearFunction(Vertices& object)
    {
        glDeleteBuffers(1, &object.vbo);
#ifdef APPLE
        glDeleteVertexArraysAPPLE(1, &object.vao);
#else
        glDeleteVertexArrays(1, &object.vao);
#endif
    }
    
    /**
     * cache implementation
     */
    Cache<Vertices> g_vertices_cache(meshCacheLoadingFunction, meshCacheClearFunction);
    
    /**
     * destructor
     */
    Mesh::~Mesh()
    {
        clear();
    }
    
    /**
     * load vertices into memory
     */
    void Mesh::init(const std::string&& id)
    {
        m_vertices_id = id;
        g_vertices_cache.add(id);
    }

    void Mesh::init(const std::string& id)
    {
        m_vertices_id = id;
        g_vertices_cache.add(id);
    }
    
    /**
     * check if mesh has any vertices or 
     * if it hasn't been initialized
     */
    bool Mesh::empty() const
    {
        return m_vertices_id == "";
    }
        
    /**
     * draw mesh
     */
    void Mesh::draw()
    {
        if(empty())
        {
            return;
        }

        const Vertices* data = g_vertices_cache.peek(m_vertices_id);

        if (data == nullptr)
        {
            data = g_vertices_cache.get(m_vertices_id);
        }
        
#ifdef APPLE
        glBindVertexArrayAPPLE(data->vao);
#else
        glBindVertexArray(data->vao);
#endif
        glDrawArrays(GL_TRIANGLES, 0, data->size);
        
#ifdef APPLE
        glBindVertexArrayAPPLE(0);
#else
        glBindVertexArray(0);
#endif
    }

    /**
     * get vertices
     */
    const std::vector<Vertex>* Mesh::vertices()
    {
        const Vertices* vertices = g_vertices_cache.peek(m_vertices_id);

        if (vertices == nullptr)
        {
            vertices = g_vertices_cache.get(m_vertices_id);
        }

        return &(vertices->data);
    }

    /**
     * get material
     */
    const Material* Mesh::material()
    {
        const Vertices* vertices = g_vertices_cache.peek(m_vertices_id);

        if (vertices == nullptr)
        {
            vertices = g_vertices_cache.get(m_vertices_id);
        }

        if(vertices->has_material)
        {
            return &vertices->material;
        }
        else
        {
            return nullptr;
        }
    }

    /**
     * deletes the data saved in ram
     */
    void Mesh::discardVertices()
    {
        Vertices* vertices = const_cast<Vertices*>(g_vertices_cache.peek(m_vertices_id));

        if (vertices != nullptr)
        {
            vertices->data.clear();
        }
    }

    /**
     * extract triangle from vertices
     */
    Triangle Mesh::constructTriangle(u32 vert_index_start)
    {
        const Vertices* vertices = g_vertices_cache.peek(m_vertices_id);
        if (vertices == nullptr)
        {
            vertices = g_vertices_cache.get(m_vertices_id);
        }

        if (vert_index_start + 2 >= vertices->data.size())
        {
            throw std::runtime_error("Mesh::constructTriangle() error: not enought vertices");
        }

        return { vertices->data[vert_index_start + 0].pos, 
                 vertices->data[vert_index_start + 1].pos,
                 vertices->data[vert_index_start + 2].pos };
    }
    
    /**
     * deallocate vertices
     */
    void Mesh::clear()
    {
        if(empty())
        {
            return;
        }
        
        g_vertices_cache.del(m_vertices_id);
    }

    void Mesh::bindVertexPositionWithShader(const Shader& shader, const char* attribute_name)
    {
        const Vertices* vertices = (g_vertices_cache.peek(m_vertices_id));
        if (vertices == nullptr)
        {
            vertices = (g_vertices_cache.get(m_vertices_id));
        }

        u32 attribute_index = shader.getAttributeIndex(attribute_name);

        glBindVertexArray(vertices->vao);
        glBindBuffer(GL_ARRAY_BUFFER, vertices->vbo);

        glEnableVertexAttribArray(attribute_index);
        glVertexAttribPointer(attribute_index, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    void Mesh::bindVertexNormalWithShader(const Shader& shader, const char* attribute_name)
    {
        const Vertices* vertices = (g_vertices_cache.peek(m_vertices_id));
        if (vertices == nullptr)
        {
            vertices = (g_vertices_cache.get(m_vertices_id));
        }

        u32 attribute_index = shader.getAttributeIndex(attribute_name);

        glBindVertexArray(vertices->vao);
        glBindBuffer(GL_ARRAY_BUFFER, vertices->vbo);

        glEnableVertexAttribArray(attribute_index);
        glVertexAttribPointer(attribute_index, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nor));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    void Mesh::bindVertexUVWithShader(const Shader& shader, const char* attribute_name)
    {
        const Vertices* vertices = (g_vertices_cache.peek(m_vertices_id));
        if (vertices == nullptr)
        {
            vertices = (g_vertices_cache.get(m_vertices_id));
        }

        u32 attribute_index = shader.getAttributeIndex(attribute_name);

        glBindVertexArray(vertices->vao);
        glBindBuffer(GL_ARRAY_BUFFER, vertices->vbo);

        glEnableVertexAttribArray(attribute_index);
        glVertexAttribPointer(attribute_index, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};