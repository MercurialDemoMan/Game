#pragma once

#include <unordered_map>
#include <string>

#include "File.hpp"

namespace Engine3D
{
	/**
	 * custom key = value database
	 */
	class Save
	{
	public:
	
		/**
		 * constructors
		 */
		Save() {}
		Save(const char* save_path);
		Save(const std::string& save_path);
		
		/**
		 * add data into save data
		 */
		void addNumber(const char* id, int value);
		void addNumber(const std::string& id, int value);
		void addNumber(const char* id, float value);
		void addNumber(const std::string& id, float value);
		void addString(const char* id, const char* value);
		void addString(const std::string& id, const char* value);
		void addString(const char* id, const std::string& value);
		void addString(const std::string& id, const std::string& value);
		
		/**
		 * erase item from save data
		 */
		void erase(const char* id);
		void erase(const std::string&& id);
		void erase(const std::string& id);
		
		/**
		 * explicit constructors
		 */
		void load(const char* save_path);
		void load(const std::string&& save_path);
		void load(const std::string& save_path);
		
		/**
		 * flush data on disc
		 */
		void save();
		
	private:
	
		struct SaveObject
		{
			enum class Type
			{
				Int,
				Float,
				String
			} type;
			
			int         int_value { 0 };
			float       flt_value { 0 };
			std::string str_value;
		};
		
		std::unordered_map<std::string, SaveObject> m_save_map;
		
		File m_file_handle;
	};
};