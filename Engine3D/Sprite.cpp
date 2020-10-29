#include "Sprite.hpp"
#include "Shader.hpp"

#include <GL/glew.h>
#include <SDL2/SDL_ttf.h>

#include <glm/gtc/type_ptr.hpp>

namespace Engine3D
{
    void Sprite::draw(Camera& cam)
    {
        static Shader image_shader
        (
            ShaderSource
            (
                R"(#version 120
                         
                varying vec2 coord;
                         
                void main() 
                {
                    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
                    coord       = gl_MultiTexCoord0.xy;
                })"
            ),
            ShaderSource
            (
                R"(#version 120
                         
                varying vec2 coord;
                uniform sampler2D image;
                uniform vec4 color;
                         
                void main() 
                {
                    gl_FragColor = texture2D(image, coord) * color;
                })"
            )
        );

        image_shader.use();

        image_shader.setTexture2D(texture().getID(), "image");
        image_shader.set4f(glm::vec4(1), "color");

        if (m_texture.empty())
        {
            return;
        }

        glPushMatrix();

        glTranslatef(m_pos.x, m_pos.y, m_pos.z);

        glMultMatrixf(glm::value_ptr(cam.getRotationMatrix()));

        glm::vec2 texture_dims = m_texture.getDims();

        u32 current_frame_as_integer = std::floor(m_current_frame);
        glm::vec2 frame_index = glm::vec2((current_frame_as_integer % m_frame_dims.x) * (texture_dims.x / m_frame_dims.x) / texture_dims.x,
                                          (current_frame_as_integer / m_frame_dims.x) * (texture_dims.y / m_frame_dims.y) / texture_dims.y);

        glm::vec2 frame_dims = glm::vec2((1.0 / m_frame_dims.x),
                                         -(1.0 / m_frame_dims.y));

        glBegin(GL_QUADS);

        //TODO: make scale.y compatible
        if (m_scale.x > 0.0)
        {
            glTexCoord2f(frame_index.x + frame_dims.x, frame_index.y);
            glVertex3f(m_scale.x * m_dims.x, -m_scale.y * m_dims.y, 0);
            glTexCoord2f(frame_index.x + frame_dims.x, frame_index.y - frame_dims.y);
            glVertex3f(m_scale.x * m_dims.x, m_scale.y * m_dims.y, 0);

            glTexCoord2f(frame_index.x, frame_index.y - frame_dims.y);
            glVertex3f(-m_scale.x * m_dims.x, m_scale.y * m_dims.y, 0);
            glTexCoord2f(frame_index.x, frame_index.y);
            glVertex3f(-m_scale.x * m_dims.x, -m_scale.y * m_dims.y, 0);
        }
        else
        {
            glTexCoord2f(frame_index.x, frame_index.y);
            glVertex3f(-m_scale.x * m_dims.x, -m_scale.y * m_dims.y, 0);
            glTexCoord2f(frame_index.x, frame_index.y - frame_dims.y);
            glVertex3f(-m_scale.x * m_dims.x, m_scale.y * m_dims.y, 0);

            glTexCoord2f(frame_index.x + frame_dims.x, frame_index.y - frame_dims.y);
            glVertex3f(m_scale.x * m_dims.x, m_scale.y * m_dims.y, 0);
            glTexCoord2f(frame_index.x + frame_dims.x, frame_index.y);
            glVertex3f(m_scale.x * m_dims.x, -m_scale.y * m_dims.y, 0);
        }
        
        

        glEnd();

        glPopMatrix();

        image_shader.unuse();
    }
}