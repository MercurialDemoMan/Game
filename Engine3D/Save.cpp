#include "Save.hpp"

#include "System.hpp"

namespace Engine3D
{
	/**
	 * constructors
	 */
	Save::Save(const char* save_path)
	{
		load(save_path);
	}
	Save::Save(const std::string& save_path)
	{
		load(save_path);
	}
	
	/**
	 * erase item from save data
	 */
	void Save::erase(const char* id)
	{
		m_save_map.erase(id);
	}
	void Save::erase(const std::string&& id)
	{
		m_save_map.erase(id);
	}
	void Save::erase(const std::string& id)
	{
		m_save_map.erase(id);
	}
	
	/**
	 * explicit constructors
	 */
	void Save::load(const char* save_path)
	{
		if(m_file_handle.opened())
		{
			//TODO: error
			return;
		}
		
		m_file_handle.open(System::getFullPath(save_path));
		
		if(m_file_handle.failed())
		{
			//TODO: error
			return;
		}
		
		// parse file
		std::vector<u8> data = m_file_handle.read();
		std::string     line;
		
		for(u32 i = 0; i < data.size(); i++)
		{
			if(data[i] != '\n')
			{
				line += data[i];
			}
			else
			{
				std::string id;
				id.resize(64);
				
				SaveObject object;
				object.str_value.resize(64);
				
				// try to parse int
				if(std::sscanf(line.c_str(), "%64s = %i", &id[0], &object.int_value) != 2)
				{
					// try to parse float
					if(std::sscanf(line.c_str(), "%64s = %f", &id[0], &object.flt_value) != 2)
					{
						// try to parse string
						if(std::sscanf(line.c_str(), "%64s = %64s", &id[0], &object.str_value[0]) != 2)
						{
							//TODO: error
						}
						else
						{
							m_save_map.insert(std::pair<std::string, SaveObject>(id, object));
						}
					}
					else
					{
						m_save_map.insert(std::pair<std::string, SaveObject>(id, object));
					}
				}
				else
				{
					m_save_map.insert(std::pair<std::string, SaveObject>(id, object));
				}
				
				line.clear();
			}
		}
	}
	void Save::load(const std::string& save_path)
	{
		load(save_path.c_str());
	}
	void Save::load(const std::string&& save_path)
	{
		load(save_path.c_str());
	}

	/**
		 * add data into save data
		 */
	void Save::addNumber(const char* id, int value)
	{
		m_save_map.insert(std::pair<std::string, SaveObject>(id, { SaveObject::Type::Int, value } ));
	}
	void Save::addNumber(const std::string& id, int value)
	{
		m_save_map.insert(std::pair<std::string, SaveObject>(id, { SaveObject::Type::Int, value } ));
	}
	void Save::addNumber(const char* id, float value)
	{
		m_save_map.insert(std::pair<std::string, SaveObject>(id, { SaveObject::Type::Float, 0, value }));
	}
	void Save::addNumber(const std::string& id, float value)
	{
		m_save_map.insert(std::pair<std::string, SaveObject>(id, { SaveObject::Type::Float, 0, value }));
	}
	void Save::addString(const char* id, const char* value)
	{
		m_save_map.insert(std::pair<std::string, SaveObject>(id, { SaveObject::Type::String, 0, 0.0f, value }));
	}
	void Save::addString(const std::string& id, const char* value)
	{
		m_save_map.insert(std::pair<std::string, SaveObject>(id, { SaveObject::Type::String, 0, 0.0f, value }));
	}
	void Save::addString(const char* id, const std::string& value)
	{
		m_save_map.insert(std::pair<std::string, SaveObject>(id, { SaveObject::Type::String, 0, 0.0f, value }));
	}
	void Save::addString(const std::string& id, const std::string& value)
	{
		m_save_map.insert(std::pair<std::string, SaveObject>(id, { SaveObject::Type::String, 0, 0.0f, value }));
	}

	/**
	 * flush data on disc
	 */
	void Save::save()
	{
		if(m_file_handle.failed())
		{
			//TODO: error
			return;
		}
		
		for(auto& it : m_save_map)
		{
			switch(it.second.type)
			{
				case SaveObject::Type::Int:
				{
					m_file_handle.write(it.first + " = " + std::to_string(it.second.int_value) + "\n");
					break;
				}
				
				case SaveObject::Type::Float:
				{
					m_file_handle.write(it.first + " = " + std::to_string(it.second.flt_value) + "\n");
					break;
				}
				
				case SaveObject::Type::String:
				{
					m_file_handle.write(it.first + " = " + it.second.str_value + "\n");
					break;
				}
			}
		}
	}
};















