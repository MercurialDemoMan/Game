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
#include "File.hpp"

#include <stdexcept>

namespace Engine3D
{
    /**
     * constructors
     */
    File::File(const char* path, OpenMode mode/* = Mode::Read*/)
    {
        open(path, mode);
    }
    File::File(const std::string& path, OpenMode mode/* = Mode::Read*/)
    {
        open(path.c_str(), mode);
    }
    /**
     * descructor
     */
    File::~File()
    {
        close();
    }
        
    /**
     * explicitly open new file
     */
    void File::open(const char* path, OpenMode mode/* = Mode::Read*/)
    {
        //try to open file as a directory
        m_dir_handle = opendir(path);
            
        if(m_dir_handle != nullptr)
        {
            m_path = std::string(path);
            if(m_path[m_path.size() - 1] == '/' || m_path[m_path.size() - 1] == '\\')
            {
                m_path.pop_back();
            }
            return;
        }
            
        //try to open file as a normal file
        m_file_handle = fopen(path, mode == OpenMode::Read ? "rb" : "wb");
            
        if(m_file_handle == nullptr)
        {
            //TODO: error
            return;
        }
            
        m_path = std::string(path);
        if(m_path[m_path.size() - 1] == '/' || m_path[m_path.size() - 1] == '\\')
        {
            m_path.pop_back();
        }
    }
    void File::open(const std::string& path, OpenMode mode/* = Mode::Read*/)
    {
        open(path.c_str());
    }
        
    /**
        * explicitly close file
        */
    void File::close()
    {
        if(m_dir_handle != nullptr)
        {
            closedir(m_dir_handle);
            m_dir_handle = nullptr;
        }
        if(m_file_handle != nullptr)
        {
            fclose(m_file_handle);
            m_file_handle = nullptr;
        }
        m_path.clear();
    }
        
    /**
        * get file size
        */
    u64 File::size()
    {
        if(m_file_handle == nullptr)
        {
            return 0;
        }
            
        u64 backup_pos = ftell(m_file_handle);
        fseek(m_file_handle, 0, SEEK_END);
        u64 file_size  = ftell(m_file_handle);
            
        // restore file pos
        fseek(m_file_handle, static_cast<long>(backup_pos), SEEK_SET);
            
        return file_size;
    }
        
    /**
        * read portion of file
        *
        * @arg num_bytes if set to 0, function will read whole file
        */
    std::vector<u8> File::read(u64 num_bytes/* = 0*/)
    {        
        std::vector<u8> result;
            
        if(m_file_handle == nullptr)
        {
            throw std::runtime_error("File::read() error: file not opened");
        }
            
        // read whole file
        if(num_bytes == 0)
        {
            fseek(m_file_handle, 0, SEEK_SET);
                
            u64 result_size = this->size();
                
            if(result_size > 0xFFFFFF)
            {
                //TODO: warning, we are probably doing something wrong
            }
                
            result.resize(static_cast<u32>(result_size));
            fread(&result[0], 1, static_cast<size_t>(result_size), m_file_handle);
                
            fseek(m_file_handle, 0, SEEK_SET);
        }
        else
        {
            result.resize(static_cast<u32>(num_bytes));
            fread(&result[0], 1, static_cast<size_t>(num_bytes), m_file_handle);
        }

        return result;
    }

    /**
        * read text
        */
    std::string File::readText(u64 num_bytes/* = 0*/)
    {
        auto data = read();

        return std::string(data.begin(), data.end());
    }
        
    /**
        * read 1 byte
        *
        * @return read byte or EOF
        */
    int File::readByte()
    {
        if(m_file_handle == nullptr)
        {
            return File::Eof;
        }
            
        return fgetc(m_file_handle);
    }
        
    /**
        * write data itno a file
        */
    u64 File::write(const char* data, u64 size)
    {
        if(failed())
        {
            //TODO: error
            return 0;
        }
            
        return fwrite(data, 1, static_cast<size_t>(size), m_file_handle);
    }
    u64 File::write(const std::string& data)
    {
        return write(data.c_str(), data.size());
    }
    u64 File::write(const std::string&& data)
    {
        return write(data.c_str(), data.size());
    }
    u64 File::write(const std::vector<u8>& data)
    {
        return write((const char*)&data[0], data.size());
    }
        
    /**
        * write 1 byte
        */
    u64 File::writeByte(u8 byte)
    {
        return write((const char*)&byte, 1);
    }
        
    /**
        * if we opened a directory we can list all file paths inside
        */
    std::vector<std::string> File::list()
    {
        std::vector<std::string> result;
            
        if(m_dir_handle == nullptr)
        {
            return result;
        }
            
        dirent* entry = nullptr;
            
        while((entry = readdir(m_dir_handle)))
        {
            result.emplace_back(entry->d_name);
        }
            
        return result;
    }
};












