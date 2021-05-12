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

#include "Macros.hpp"

#if ENGINE3D_PLATFORM == WINDOWS
#include "dirent_win.h"
#else
#include <dirent.h>
#endif

#include <vector>
#include <string>
#include <cstdio>

#include "Types.hpp"

namespace Engine3D
{

    /**
        * file and directory handler
        */
    class File
    {
    public:
            
        /**
            * open mode
            */
        enum class OpenMode
        {
            Read,
            Write
        };
            
        /**
            * End of file identificator
            */
        constexpr static int Eof = EOF;
        
        /**
            * constructors
            */
        File(){}
        File(const char* path, OpenMode mode = OpenMode::Read);
        File(const std::string& path, OpenMode mode = OpenMode::Read);
        /**
            * descructor
            */
        ~File();
           
        /**
            * explicitly open new file
            */
        void open(const char* path, OpenMode mode = OpenMode::Read);
        void open(const std::string& path, OpenMode mode = OpenMode::Read);
            
        /**
            * explicitly close file
            */
        void close();
            
        /**
            * get file size
            */
        u64 size();
            
        /**
            * check if file is opened
            */
        bool opened() { return m_file_handle != nullptr || m_dir_handle != nullptr; }
        bool failed() { return m_file_handle == nullptr && m_dir_handle == nullptr; }
            
        /**
            * read raw portion of file
            *
            * @arg num_bytes if set to 0, function will read whole file
            */
        std::vector<u8> read(u64 num_bytes = 0);

        /**
            * read text
            */
        std::string readText(u64 num_bytes = 0);
            
        /**
            * read 1 byte
            *
            * @return read byte or EOF
            */
        int readByte();
            
        /**
            * write data itno a file
            */
        u64 write(const char* data, u64 size);
        u64 write(const std::string& data);
        u64 write(const std::string&& data);
        u64 write(const std::vector<u8>& data);
            
        /**
            * write 1 byte
            */
        u64 writeByte(u8 byte);

            
        /**
            * did we open a file?
            */
        bool isFile() { return m_file_handle != nullptr; }
        /**
            * did we open a directory?
            */
        bool isDir()  { return m_dir_handle  != nullptr; }
            
        /**
            * if we opened a directory we can list all file paths inside
            */
        std::vector<std::string> list();
            
        /**
            * get the path of the file
            */
        const std::string& getPath() { return m_path; }

        /**
            * get containing folder of the file
            */
        std::string getFolder()
        {
            std::string file_path = getPath();
            return file_path.substr(0, file_path.find_last_of("/\\"));
        }
            
    private:
        
        FILE* m_file_handle { nullptr };
        DIR*  m_dir_handle  { nullptr };
            
        std::string m_path  { "" };
    };

};