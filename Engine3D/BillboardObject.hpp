#pragma once

#include <glm/glm.hpp>

#include "Texture.hpp"
#include "Camera.hpp"

namespace Engine3D
{
    /**
     * 3D plane that will always face the camera
     */
    class BillboardObject
    {
    public:

        /**
         * constructors
         */
        BillboardObject() {}
        BillboardObject(glm::vec3 pos) : m_pos(pos) {}
        BillboardObject(glm::vec3 pos, const char* tex_id) : m_pos(pos)
        {
            init(pos, tex_id);
        }
        BillboardObject(glm::vec3 pos, const std::string& tex_id) : m_pos(pos)
        {
            init(pos, tex_id.c_str());
        }

        /**
         * explicit initializers
         */
        void init(glm::vec3 pos) { m_pos = pos; }
        void init(glm::vec3 pos, const char* tex_id)
        { 
            m_pos = pos; 
            m_texture.init(tex_id); 
            m_dims = m_texture.getDims();
        }
        void init(glm::vec3 pos, const std::string& tex_id) 
        { 
            init(pos, tex_id.c_str());
        }

        /**
         * access attributes
         */
        glm::vec3& pos() { return m_pos; }
        glm::vec2& scale() { return m_scale; }
        glm::vec2& dims()   { return m_dims; }
        Texture&   texture() { return m_texture; }

        /**
         * draw 3D object
         */
        void draw(Camera& cam);

    protected:

        glm::vec3 m_pos   { 0 };
        glm::vec2 m_scale { 1, 1 };
        glm::vec2 m_dims  { 1, 1 };
        Texture   m_texture;
    };
};