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
#include "Texture.hpp"

#include "Macros.hpp"

#include <GL/glew.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>


#include "File.hpp"
#include "Cache.hpp"

namespace Engine3D
{
    struct InternalTexture
    {
        u32 width;
        u32 height;
        u32 id;
    };

    /**
     * load texture from image
     */
    InternalTexture textureCacheLoadingFunction(File& input_file)
    {
        std::vector<u8> buffer = input_file.read();
        
        //load image
        SDL_RWops*   handle = SDL_RWFromMem(&buffer[0], buffer.size());
        SDL_Surface* img    = IMG_Load_RW(handle, 0);
        
        if(img == nullptr)
        {
            return { 0, 0, 0 };
        }
        
        //convert image format
        SDL_Surface* img_true = SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_ABGR8888, 0);
        
        //create texture and copy data into it
        u32 tex = 0;
        u32 width = img_true->w;
        u32 height = img_true->h;

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_true->w, img_true->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_true->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        glBindTexture(GL_TEXTURE_2D, 0);
        
        SDL_FreeSurface(img);
        SDL_FreeSurface(img_true);
        SDL_RWclose(handle);
        
        return { width, height, tex };
    }
    /**
     * clear texture
     */
    void textureCacheClearFunction(InternalTexture& object)
    {
        glDeleteTextures(1, &object.id);
    }
    /**
     * cache implementation
     */
    Cache<InternalTexture> g_texture_cache(textureCacheLoadingFunction, textureCacheClearFunction);
    
    /**
     * load texture into memory
     */
    void Texture::init(const char* id)
    {
        if (id == nullptr)
        {
            return;
        }

        m_texture_id = id;

        try
        {
            g_texture_cache.add(id);
        }
        catch (std::runtime_error& e)
        {
            std::printf("Texture::init() error: %s - %s\n", e.what(), id);
        }
    }
    
    /**
     * delete texture
     */
    void Texture::clean()
    {
        g_texture_cache.del(m_texture_id);
    }

    /**
     * check if the texture is initialized
     */
    bool Texture::empty()
    {
        if (m_texture_id.empty())
        {
            return true;
        }

        const InternalTexture* tex = g_texture_cache.peek(m_texture_id);

        if (tex == nullptr)
        {
            tex = g_texture_cache.get(m_texture_id);
        }

        if (tex->id == 0)
        {
            return true;
        }

        return false;
    }
    
    /**
     * get texture ID from OpenGL
     */
    u32 Texture::getID()
    {
        if (m_texture_id.empty())
        {
            return -1;
        }

        const InternalTexture* tex = g_texture_cache.peek(m_texture_id);

        if (!tex)
        {
            tex = g_texture_cache.get(m_texture_id);
        }

        return tex->id;
    }

    /**
     * get texture width and height
     */
    glm::vec2 Texture::getDims()
    {
        if (m_texture_id.empty())
        {
            return glm::vec2(0);
        }

        const InternalTexture* tex = g_texture_cache.peek(m_texture_id);

        if (!tex)
        {
            tex = g_texture_cache.get(m_texture_id);
        }

        return glm::vec2(tex->width, tex->height);
    }
};