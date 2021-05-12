/*
This file is part of Engine3D.

Engine3D is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Engine3D is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Engine3D.  If not, see <https://www.gnu.org/licenses/>.
*/
/*
This file is part of Engine3D.

Engine3D is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Engine3D is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Engine3D.  If not, see <https://www.gnu.org/licenses/>.
*/
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
        BillboardObject(const glm::vec3& pos) : m_pos(pos) {}
        BillboardObject(const glm::vec3& pos, const char* tex_id) : m_pos(pos)
        {
            init(pos, tex_id);
        }
        BillboardObject(const glm::vec3& pos, const std::string& tex_id) : m_pos(pos)
        {
            init(pos, tex_id.c_str());
        }

        /**
         * explicit initializers
         */
        void init(const glm::vec3& pos) { m_pos = pos; }
        void init(const glm::vec3& pos, const char* tex_id)
        { 
            m_pos = pos; 
            m_texture.init(tex_id);
            m_dims = m_texture.getDims();
        }
        void init(const glm::vec3& pos, const std::string& tex_id) 
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
        virtual void draw(Camera& cam);

    protected:

        glm::vec3 m_pos   { 0 };
        glm::vec2 m_scale { 1, 1 };
        glm::vec2 m_dims  { 1, 1 };
        Texture   m_texture;
    };
};