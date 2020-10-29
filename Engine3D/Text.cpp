#include "Text.hpp"

#include "Macros.hpp"

#include <GL/glew.h>
#include <SDL2/SDL_ttf.h>

#include "System.hpp"
#include "Shader.hpp"

namespace Engine3D
{
    Text::Text(Engine3D::Game& game, const std::string& font_path, u32 font_size/* = DefaultFontSize*/)
    {
        init(game, font_path);
    }
    
    Text::~Text()
    {
        clear();
    }

    /**
     * clear
     */
    void Text::clear()
    {
        if (m_texture != 0)
        {
            glDeleteTextures(1, &m_texture);
            m_texture = 0;
        }

        if (m_font != nullptr)
        {
            TTF_CloseFont(m_font);
            m_font = nullptr;
        }
    }

    /**
     * explicit constructor
     */
    void Text::init(Engine3D::Game& game, const std::string& font_path, u32 font_size/* = DefaultFontSize*/)
    {
        //load font into memory
        m_font_path = System::getFullPath(font_path);

        //set font size
        m_font_size = font_size;

        //initialize font
        m_font = TTF_OpenFont(m_font_path.c_str(), m_font_size);

        if (m_font == nullptr)
        {
            throw std::runtime_error(TTF_GetError());
        }

        //store window dimensions
        m_screen_dims = game.getDims();
}
    
    /**
     * update text texture
     */
    void Text::update(const char* text)
    {
        //render the text
        SDL_Surface* text_surface = TTF_RenderUTF8_Blended(m_font, text, m_font_color);
        
        if(text_surface == nullptr)
        {
            printf("SDLText::update TTF_RenderText_Solid error: %s\n", SDL_GetError());
            SDL_FreeSurface(text_surface);
            return;
        }

        if(m_texture == 0)
        {
            glGenTextures(1, &m_texture);
        }
        
        //update GPU texture data
        glBindTexture(GL_TEXTURE_2D, m_texture);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text_surface->w, text_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, text_surface->pixels);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //TODO: this is fucking up the line drawing
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_NEAREST; GL_LINEAR
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //GL_NEAREST; GL_LINEAR
        
        m_font_width  = text_surface->w;
        m_font_height = text_surface->h;
        
        glBindTexture(GL_TEXTURE, 0);

        SDL_FreeSurface(text_surface);
    }
    void Text::update(const std::string&& text)
    {
        update(text.c_str());
    }

    void Text::draw()
    {
        static Engine3D::Shader font_shader;
        static bool             font_shader_init = false;

        if (font_shader_init == false)
        {
            font_shader.init("data/shaders/image.vert", "data/shaders/image.frag");
            font_shader_init = true;
        }

        //transform coordinates
        glm::vec2 pos = m_pos / m_screen_dims;
        glm::vec2 dim = glm::vec2(m_font_width, m_font_height) / m_screen_dims;

        font_shader.use();

        font_shader.setTexture2D(m_texture, "image");
        font_shader.set3f(glm::vec3(1), "color");

        glBegin(GL_QUADS);
        glTexCoord2f(1, 0);
        //glVertex3f(1, 1, 0); 
        glVertex3f(pos.x + dim.x / 2.0f, pos.y + dim.y / 2.0f, 0);

        glTexCoord2f(0, 0);
        //glVertex3f(-1, 1, 0);
        glVertex3f(pos.x - dim.x / 2.0f, pos.y + dim.y / 2.0f, 0);

        glTexCoord2f(0, 1);
        //glVertex3f(-1, -1, 0);
        glVertex3f(pos.x - dim.x / 2.0f, pos.y - dim.y / 2.0f, 0);

        glTexCoord2f(1, 1);
        //glVertex3f(1, -1, 0);
        glVertex3f(pos.x + dim.x / 2.0f, pos.y - dim.y / 2.0f, 0);
        glEnd();

        font_shader.unuse();
    }
    
    Text& Text::operator=(const char* text)
    {
        this->update(text);
        return *this;
    }

    /**
     * resize font, requires to reopen the font
     */
    void Text::resize(u32 font_size)
    {
        //close the old font
        clear();
        
        //set font size
        m_font_size = font_size;

        //initialize font
        m_font = TTF_OpenFont(m_font_path.c_str(), m_font_size);

        if (m_font == nullptr)
        {
            throw std::runtime_error(TTF_GetError());
        }
    }
};