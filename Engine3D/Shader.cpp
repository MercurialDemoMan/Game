#include "Shader.hpp"

#include <stdexcept>

#include "Macros.hpp"

#include <GL/glew.h>

#include <SDL2/SDL.h>

namespace Engine3D
{
    /**
     * compile fragment shader
     */
    static u32 compileFragmentShader(const char* source)
    {
        u32 program = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(program, 1, &source, nullptr);
        glCompileShader(program);

        char error_buffer[128] = { 0 };

        glGetShaderInfoLog(program, 128, nullptr, error_buffer);

        if (error_buffer[0] != '\0')
        {
            //TODO: error
            throw std::runtime_error(error_buffer);
            return -1;
        }

        return program;
    }


    /**
     / compile vertex shader
     */
    static u32 compileVertexShader(const char* source)
    {
        u32 program = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(program, 1, &source, nullptr);
        glCompileShader(program);

        char error_buffer[128] = { 0 };

        glGetShaderInfoLog(program, 128, nullptr, error_buffer);

        if (error_buffer[0] != '\0')
        {
            //TODO: error
            throw std::runtime_error(error_buffer);
            return -1;
        }

        return program;
    }

    /**
     * fragment program cache manager
     */
    u32 cacheFragmentLoadingFunction(File& input_file)
    {
        return compileFragmentShader(reinterpret_cast<const char*>(&input_file.readText()[0]));
    }
    void cacheFragmentClearFunction(u32& program)
    {
        if(program != -1) { glDeleteShader(program); }
    }
    Cache<u32> g_fragment_program_cache(cacheFragmentLoadingFunction, cacheFragmentClearFunction);
    
    /**
     * vertex program cache manager
     */
    u32 cacheVertexLoadingFunction(File& input_file)
    {
        return compileVertexShader(reinterpret_cast<const char*>(&input_file.readText()[0]));
    }
    void cacheVertexClearFunction(u32& program)
    {
        if(program != -1) { glDeleteShader(program); }
    }
    Cache<u32> g_vertex_program_cache(cacheVertexLoadingFunction, cacheVertexClearFunction);
    
    /**
     * constructors
     */
    Shader::Shader(const char* vertex_shader_path, const char* fragment_shader_path)
    {
        init(vertex_shader_path, fragment_shader_path);
    }
    Shader::Shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
    {
        init(vertex_shader_path, fragment_shader_path);
    }
    Shader::Shader(ShaderSource vertex_shader, ShaderSource fragment_shader)
    {
        u32 vertex_shader_id = compileVertexShader(vertex_shader.source());
        u32 fragment_shader_id = compileFragmentShader(fragment_shader.source());

        if (vertex_shader_id == -1 || fragment_shader_id == -1)
        {
            //TODO: error
            return;
        }

        m_program = glCreateProgram();

        glAttachShader(m_program, vertex_shader_id);
        glAttachShader(m_program, fragment_shader_id);
        glLinkProgram(m_program);
    }

    /**
     * destructor
     */
    Shader::~Shader()
    {
        if(m_program != -1)
        {
            if(!m_vertex_id.empty()) 
            {
                glDetachShader(m_program, *g_vertex_program_cache.get(m_vertex_id));
                g_vertex_program_cache.del(m_vertex_id);
            }
            if(!m_fragment_id.empty()) 
            { 
                glDetachShader(m_program, *g_fragment_program_cache.get(m_vertex_id));
                g_fragment_program_cache.del(m_fragment_id);
            }
            
            glDeleteProgram(m_program);
        }
    }
    
    /**
     * state that we wanto to use this program
     */
    void Shader::use()
    {
        glUseProgram(m_program);
        m_free_texture = 0;
    }
    
    /**
     * stop using this program
     */
    void Shader::unuse()
    {
        glUseProgram(0);
        m_free_texture = 0;
    }
    
    /**
     * compile shader program
     */
    void Shader::init(const char* vertex_shader_path, const char* fragment_shader_path)
    {
        u32 vertex_shader   = *g_vertex_program_cache.get(vertex_shader_path);
        u32 fragment_shader = *g_fragment_program_cache.get(fragment_shader_path);
        
        if(vertex_shader == -1 || fragment_shader == -1)
        {
            //TODO: error
            return;
        }
        
        m_program = glCreateProgram();
        
        glAttachShader(m_program, vertex_shader);
        glAttachShader(m_program, fragment_shader);
        glLinkProgram(m_program);
    }
    
    /**
     * setting uniforms in shaders
     */
    static u32 TextureID[] =
    {
        GL_TEXTURE0,
        GL_TEXTURE1,
        GL_TEXTURE2,
        GL_TEXTURE3,
        GL_TEXTURE4,
        GL_TEXTURE5,
        GL_TEXTURE6,
        GL_TEXTURE7,
    };
    void Shader::setTexture2D(u32 texture_id, const char* uniform_name)
    {
        if(m_free_texture == sizeof(TextureID) / sizeof(TextureID[0]) - 1)
        {
            //TODO: error
            std::printf("Shader::setTexture2D() error: reached maximum number of textures\n");
            return;
        }

        if(texture_id == 0)
        {
            //TODO: error
            std::printf("Shader::setTexture2D() error: trying to assign empty texture\n");
            return;
        }

        if(uniform_name == nullptr)
        {
            //TODO: error
            std::printf("Shader::setTexture2D() error: expected uniform name\n");
            return;
        }

        glActiveTexture(TextureID[m_free_texture]);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        set1i(m_free_texture, uniform_name);
        m_free_texture++;
    }
    void Shader::setTexture3D(u32 texture_id, const char* uniform_name)
    {
        if (m_free_texture == sizeof(TextureID) / sizeof(TextureID[0]) - 1)
        {
            //TODO: error
            return;
        }

        if (texture_id == 0)
        {
            //TODO: error
            return;
        }

        if (uniform_name == nullptr)
        {
            //TODO: error
            return;
        }

        glActiveTexture(TextureID[m_free_texture]);
        glBindTexture(GL_TEXTURE_3D, texture_id);
        set1i(m_free_texture, uniform_name);
        m_free_texture++;
    }
    void Shader::setTextureCubemap(u32 texture_id, const char* uniform_name)
    {
        if (m_free_texture == sizeof(TextureID) / sizeof(TextureID[0]) - 1)
        {
            //TODO: error
            std::printf("Shader::setTextureCubemap() error: reached maximum number of textures\n");
            return;
        }

        if (texture_id == 0)
        {
            //TODO: error
            std::printf("Shader::setTextureCubemap() error: trying to assign empty texture\n");
            return;
        }

        if (uniform_name == nullptr)
        {
            //TODO: error
            std::printf("Shader::setTextureCubemap() error: expected uniform name\n");
            return;
        }

        glActiveTexture(TextureID[m_free_texture]);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
        set1i(m_free_texture, uniform_name);
        m_free_texture++;
    }
    void Shader::set1i(int value, const char* uniform_name) 
    {
    glUniform1i(glGetUniformLocation(m_program, uniform_name), value);
    }
    void Shader::set1f(float value, const char* uniform_name)
    {
        glUniform1f(glGetUniformLocation(m_program, uniform_name), value);
    }
    void Shader::set2f(glm::vec2 value, const char* uniform_name) 
    {
        glUniform2f(glGetUniformLocation(m_program, uniform_name), value.x, value.y);
    }
    void Shader::set3f(glm::vec3 value, const char* uniform_name) 
    {
        glUniform3f(glGetUniformLocation(m_program, uniform_name), value.x, value.y, value.z);
    }
    void Shader::set4f(glm::vec4 value, const char* uniform_name) 
    {
        glUniform4f(glGetUniformLocation(m_program, uniform_name), value.x, value.y, value.z, value.w);
    }
    void Shader::set1b(bool value, const char* uniform_name) 
    {
        glUniform1i(glGetUniformLocation(m_program, uniform_name), value);
    }
    void Shader::set4x4m(glm::mat4 value, const char* uniform_name) 
    {
        glUniformMatrix4fv(glGetUniformLocation(m_program, uniform_name), 1, false, &value[0][0]);
    }
    void Shader::set1fv(float* array, unsigned size, const char* uniform_name) 
    {
        glUniform1fv(glGetUniformLocation(m_program, uniform_name), size, array);
    }
    void Shader::set1iv(int* array, unsigned size, const char* uniform_name) 
    {
        glUniform1iv(glGetUniformLocation(m_program, uniform_name), size, array);
    }

    u32 Shader::getAttributeIndex(const char* attribute_name) const
    {
        return glGetAttribLocation(m_program, attribute_name);
    }


};