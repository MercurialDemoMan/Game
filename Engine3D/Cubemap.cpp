#include "Cubemap.hpp"

#include "Macros.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>

#include "Cache.hpp"

namespace Engine3D
{
    /**
     * load texture from image
     */
    static u32  cubemapCacheLoadingFunction(File& input_file)
    {
        if(input_file.isDir() == false)
        {
            return 0;
        }
        
        std::vector<u8> reading_buffer;

        File left(input_file.getPath() + "/left.png");
        reading_buffer              = left.read();
        SDL_RWops*   handle         = SDL_RWFromMem(&reading_buffer[0], static_cast<int>(reading_buffer.size()));
        SDL_Surface* left_img       = IMG_Load_RW(handle, 0);
        SDL_Surface* left_img_conv  = SDL_ConvertSurfaceFormat(left_img, SDL_PIXELFORMAT_ABGR8888, 0);
        SDL_RWclose(handle); handle = nullptr;
        left.close();
        
        File right(input_file.getPath() + "/right.png");
        reading_buffer              = right.read();
        handle                      = SDL_RWFromMem(&reading_buffer[0], static_cast<int>(reading_buffer.size()));
        SDL_Surface* right_img      = IMG_Load_RW(handle, 0);
        SDL_Surface* right_img_conv = SDL_ConvertSurfaceFormat(right_img, SDL_PIXELFORMAT_ABGR8888, 0);
        SDL_RWclose(handle); handle = nullptr;
        right.close();
        
        File bottom(input_file.getPath() + "/bottom.png");
        reading_buffer               = bottom.read();
        handle                       = SDL_RWFromMem(&reading_buffer[0], static_cast<int>(reading_buffer.size()));
        SDL_Surface* bottom_img      = IMG_Load_RW(handle, 0);
        SDL_Surface* bottom_img_conv = SDL_ConvertSurfaceFormat(bottom_img, SDL_PIXELFORMAT_ABGR8888, 0);
        SDL_RWclose(handle); handle  = nullptr;
        bottom.close();
        
        File top(input_file.getPath() + "/top.png");
        reading_buffer              = top.read();
        handle                      = SDL_RWFromMem(&reading_buffer[0], static_cast<int>(reading_buffer.size()));
        SDL_Surface* top_img        = IMG_Load_RW(handle, 0);
        SDL_Surface* top_img_conv   = SDL_ConvertSurfaceFormat(top_img, SDL_PIXELFORMAT_ABGR8888, 0);
        SDL_RWclose(handle); handle = nullptr;
        top.close();
        
        File front(input_file.getPath() + "/front.png");
        reading_buffer              = front.read();
        handle                      = SDL_RWFromMem(&reading_buffer[0], static_cast<int>(reading_buffer.size()));
        SDL_Surface* front_img      = IMG_Load_RW(handle, 0);
        SDL_Surface* front_img_conv = SDL_ConvertSurfaceFormat(front_img, SDL_PIXELFORMAT_ABGR8888, 0);
        SDL_RWclose(handle); handle = nullptr;
        front.close();
        
        File back(input_file.getPath() + "/back.png");
        reading_buffer              = back.read();
        handle                      = SDL_RWFromMem(&reading_buffer[0], static_cast<int>(reading_buffer.size()));
        SDL_Surface* back_img       = IMG_Load_RW(handle, 0);
        SDL_Surface* back_img_conv  = SDL_ConvertSurfaceFormat(back_img, SDL_PIXELFORMAT_ABGR8888, 0);
        SDL_RWclose(handle); handle = nullptr;
        back.close();
        
        //TODO: rotate surfaces, wait... just rotate the source images 4Head
        
        //create cubemap texture
        u32 tex = 0;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
        
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        
        //copy texture data into GPU
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, left_img_conv->w,   left_img_conv->h,   0, GL_RGBA, GL_UNSIGNED_BYTE, left_img_conv->pixels);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, right_img_conv->w,  right_img_conv->h,  0, GL_RGBA, GL_UNSIGNED_BYTE, right_img_conv->pixels);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, bottom_img_conv->w, bottom_img_conv->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bottom_img_conv->pixels);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, top_img_conv->w,    top_img_conv->h,    0, GL_RGBA, GL_UNSIGNED_BYTE, top_img_conv->pixels);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, front_img_conv->w,  front_img_conv->h,  0, GL_RGBA, GL_UNSIGNED_BYTE, front_img_conv->pixels);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, back_img_conv->w,   back_img_conv->h,   0, GL_RGBA, GL_UNSIGNED_BYTE, back_img_conv->pixels);
    
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
        //cleanup
        SDL_FreeSurface(left_img);   SDL_FreeSurface(left_img_conv);
        SDL_FreeSurface(right_img);  SDL_FreeSurface(right_img_conv);
        SDL_FreeSurface(bottom_img); SDL_FreeSurface(bottom_img_conv);
        SDL_FreeSurface(top_img);    SDL_FreeSurface(top_img_conv);
        SDL_FreeSurface(front_img);  SDL_FreeSurface(front_img_conv);
        SDL_FreeSurface(back_img);   SDL_FreeSurface(back_img_conv);
        
        return tex;
    }
    /**
     * clear cubemap
     */
    static void cubemapCacheClearFunction(u32& object)
    {
        glDeleteTextures(1, &object);
    }
    
    /**
     * cache implementation
     */
    Cache<u32> g_cubemap_cache = Cache<u32>(cubemapCacheLoadingFunction, cubemapCacheClearFunction);
    
    /**
     * load texture into memory
     */
    void Cubemap::init(const std::string&& id)
    {
        m_cubemap_id = id;
        g_cubemap_cache.add(id);
    }

    /**
     * delete texture
     */
    void Cubemap::clear()
    {
        g_cubemap_cache.del(m_cubemap_id);
    }
    
    /**
     * get texture ID from OpenGL
     */
    u32 Cubemap::getID() const
    {
        return *g_cubemap_cache.get(m_cubemap_id);
    }
};