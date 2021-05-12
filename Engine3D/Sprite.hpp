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

#include <cmath>

#include "Macros.hpp"
#include "Types.hpp"
#include "BillboardObject.hpp"
#include "Camera.hpp"

namespace Engine3D
{
    class Sprite : public BillboardObject
    {
    public:
        ENGINE3D_INHERIT_CONSTRUCTORS(BillboardObject, BillboardObject);

        Sprite(const glm::vec2& pos, const char* tex_id, const glm::ivec2& frame_dims) : BillboardObject(glm::vec3(pos.x, pos.y, 1), tex_id)
        {
            m_frame_dims = frame_dims;
            m_dims /= m_frame_dims;
        }

        Sprite(const glm::vec2& pos, const std::string& tex_id, const glm::ivec2& frame_dims) : BillboardObject(glm::vec3(pos.x, pos.y, 1), tex_id)
        {
            Sprite(pos, tex_id.c_str(), frame_dims);
        }

        void setFrame(float frame) { m_current_frame = static_cast<float>(fmod(frame, m_frame_dims.x * m_frame_dims.y)); }
        void addFrame(float amount = 1) { setFrame(m_current_frame + amount); }
        void addFrame(float offset, float stop, float amount)
        {
            if (getFrame() < offset || getFrame() > offset + stop)
            {
                setFrame(offset);
            }
            setFrame(offset + std::fmod(getFrame() - offset + amount, stop));
        }
        float getFrame() { return m_current_frame;  }

        virtual void draw(Camera& cam) override;

        bool& active() { return m_active; }

        glm::vec4& color() { return m_color; }
        glm::vec4& add_color() { return m_add_color; }

    private:
        
        float m_current_frame   { 0 };
        glm::ivec2 m_frame_dims { 1 };
        bool m_active { true };
        glm::vec4 m_color { 1 };
        glm::vec4 m_add_color { 0 };
    };
}