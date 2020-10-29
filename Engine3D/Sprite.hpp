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

        Sprite(glm::vec2 pos, const char* tex_id, glm::ivec2 frame_dims) : BillboardObject(glm::vec3(pos.x, pos.y, 1), tex_id)
        {
            m_frame_dims = frame_dims;
            m_dims /= m_frame_dims;
        }

        Sprite(glm::vec2 pos, const std::string& tex_id, glm::ivec2 frame_dims) : BillboardObject(glm::vec3(pos.x, pos.y, 1), tex_id)
        {
            Sprite(pos, tex_id.c_str(), frame_dims);
        }

        void setFrame(float frame) { m_current_frame = fmod(frame, m_frame_dims.x * m_frame_dims.y); }
        void addFrame(float amount = 1) { setFrame(m_current_frame + amount); }
        void addFrame(float offset, float stop, float amount)
        {
            setFrame(offset + std::fmod(getFrame() - offset + amount, stop));
        }
        float getFrame() { return m_current_frame;  }

        void draw(Camera& cam);

    private:
        
        float m_current_frame   { 0 };
        glm::ivec2 m_frame_dims { 1 };
    };
}