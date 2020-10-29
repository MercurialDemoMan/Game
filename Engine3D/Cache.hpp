#pragma once

#include <unordered_map>
#include <functional>
#include <stdexcept>

#include "Types.hpp"
#include "File.hpp"
#include "System.hpp"

namespace Engine3D
{
    /**
     * std::unordered_map wrapper, that allows load data from file when not present in memory and destroy it when no longer necesary
     */
    template<typename T>
    class Cache
    {
    public:
    
        /**
         * constructor
         */
        Cache() {}
        Cache(std::function<T(File&)> loading_function) : m_loading_function(loading_function) {}
        Cache(std::function<T(File&)> loading_function, std::function<void(T&)> clear_function) : m_loading_function(loading_function), m_clear_function(clear_function) {} 
        
        /**
         * load data if not present, increase the reference count
         * and return the pointer to the data
         */
        const T* get(const std::string&& id) { return get(id.c_str()); }
        const T* get(const std::string& id)  { return get(id.c_str()); }
        const T* get(const char* id)
        {
            auto it = m_implementation.find(id);

            // object not in cache
            if (it == m_implementation.end())
            {
                // open input file
                File input_file(System::getFullPath(id), File::OpenMode::Read);

                if (input_file.failed())
                {
                    input_file.close();
                    throw std::runtime_error("Cache::get() cannot open input file");
                }

                // load the object from the file
                if (m_loading_function)
                {
                    m_implementation.insert(std::pair<std::string, CacheObject>(id, { 1, m_loading_function(input_file) }));
                    input_file.close();
                    return const_cast<const T*>(&(m_implementation[id].data));
                }
                else
                {
                    //TODO: error
                    input_file.close();
                    throw std::runtime_error("Cache::get() loading function not specified");
                }

                throw std::runtime_error("Cache::get() this shouldnLt be thrown\n");

                //never reached
                return nullptr;
            }
            // object in cache
            else
            {
                it->second.ref_counter++;
                return const_cast<const T*>(&it->second.data);
            }
        }
        
        /**
         * add data into cache if not present
         */
        void add(const std::string&& id) { get(id.c_str()); }
        void add(const std::string& id)  { get(id.c_str()); }
        void add(const char* id)          { get(id); }
        
        /**
         * lookup data without incrementing the reference count
         */
        const T* peek(const std::string&& id) { return peek(id.c_str()); }
        const T* peek(const std::string& id)  { return peek(id.c_str()); }
        const T* peek(const char* id)
        {
            auto it = m_implementation.find(id);

            // object not in cache
            if (it == m_implementation.end())
            {
                return nullptr;
            }
            // object in cache
            else
            {
                return const_cast<const T*>(&it->second.data);
            }
        }
        
        /**
         * delete data
         */
        void del(const std::string&& id) { return del(id.c_str()); }
        void del(const std::string& id)  { return del(id.c_str()); }
        void del(const char* id)
        {
            auto it = m_implementation.find(id);

            if (it == m_implementation.end())
            {
                return;
            }
            else
            {
                if (--it->second.ref_counter == 0)
                {
                    if (m_clear_function)
                    {
                        m_clear_function(it->second.data);
                    }

                    m_implementation.erase(it);
                }
            }
        }
    
    private:
    
        /**
         * keep track of references to the same object
         */
        struct CacheObject
        {
            u32 ref_counter;
            T   data;
        };
    
        /**
         * cache implementation
         */
        std::unordered_map<std::string, CacheObject> m_implementation;
        std::function<T(File&)>              m_loading_function;
        std::function<void(T&)>                      m_clear_function;
    };
};