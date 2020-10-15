#include "JSONDocument.hpp"

#include <functional>

#include "System.hpp"
#include "File.hpp"


namespace Engine3D
{
	/**
	 * open and parse json file
	 */
	void JSONDocument::init(const std::string& file_path)
	{
		//extract double quoted string, who needs escape sequences right?
		auto parse_string = [](std::vector<u8>& data, u32& position) -> std::string
		{
			std::string id = "";

			//skip first quotation mark
			position++;

			for(; position < data.size(); position++)
			{
				if (data[position] != '\"')
				{
					id += data[position];
				}
				else
				{
					position++;
					return id;
				}
			}

			throw std::runtime_error("JSONDocuemnt::init::parse_string() error: expected \" symbol");

			//TODO: error
			return id;
		};

		//skip whitespace characters
		auto parse_whitespace = [](std::vector<u8>& data, u32& position) -> bool
		{
			position++;

			//skip leading character
			for (; position < data.size(); position++)
			{
				if (!isspace(data[position]))
				{
					return false;
				}
			}

			return true;
		};

		//parse floating points numbers and integers, who needs exponentials right?
		auto parse_number = [](std::vector<u8>& data, u32& position) -> float
		{
			std::string num;
			bool fract_point_defined = false;

			for (; position < data.size(); position++)
			{
				if ((data[position] == '-' || data[position] == '+') && num.size() == 0)
				{
					num += data[position];
				}
				else if (data[position] >= '0' && data[position] <= '9')
				{
					num += data[position];
				}
				else if(data[position] == '.' && fract_point_defined == false)
				{
					num += data[position];
					fract_point_defined = true;
				}
				else
				{
					return static_cast<float>(strtof(num.c_str(), nullptr));
				}
			}

			return static_cast<float>(strtof(num.c_str(), nullptr));
		};

		//TODO: extract json object
		std::function<JSONObject*(std::vector<u8>&, u32&)> parse_dictionary = [&](std::vector<u8>& data, u32& position) -> JSONObject*
		{
			JSONObject* object = new JSONObject(JSONObject::Type::Dictionary);

			//skip { character
			position++;

			for (; position < data.size(); position++)
			{
				//spaces
				if (isspace(data[position]))
				{
					if (parse_whitespace(data, position))
					{
						break;
					}
				}
				//end of dictionary
				if (data[position] == '}')
				{
					return object;
				}
				//next object in dictionary
				else if (data[position] == ',')
				{
					continue;
				}
				//new anonymous dictionary
				else if (data[position] == '{')
				{
					object->anon_dict().push_back(parse_dictionary(data, position));
				}
				//key
				else if (data[position] == '"')
				{
					//extract key
					std::string id = parse_string(data, position);

					if (isspace(data[position]))
					{
						if (parse_whitespace(data, position))
						{
							break;
						}
					}

					//key with value
					if(data[position] == ':')
					{
						if (parse_whitespace(data, position))
						{
							break;
						}

						//value is dictionary
						if (data[position] == '{')
						{
							object->named_dict().insert(std::pair<std::string, JSONObject*>(id, parse_dictionary(data, position)));
						}
						//value is string
						else if (data[position] == '"')
						{
							object->named_dict().insert(std::pair<std::string, JSONObject*>(id, new JSONObject(parse_string(data, position))));
							position--;
						}
						//value is number
						else if ((data[position] >= '0' && data[position] <= '9') || data[position] == '+' || data[position] == '-')
						{
							object->named_dict().insert(std::pair<std::string, JSONObject*>(id, new JSONObject(parse_number(data, position))));
							position--;
						}
						else
						{
							//TODO: error
							throw std::runtime_error("JSONDocument::init::parse_dictionary() error: unexpected symbol");
						}
					}
					//key with no value
					else
					{
						object->named_dict().insert(std::pair<std::string, JSONObject*>(id, new JSONObject()));
					}
				}
				else
				{
					//TODO: error
					throw std::runtime_error("JSONDocument::init::parse_dictionary() error: unexpected symbol");
				}
			}

			throw std::runtime_error("JSONDocument::init::parse_dictionary() error: expected } symbol");
			//TODO: error
			return object;
		};

		File input(System::getFullPath(file_path));

		if (!input.opened())
		{
			throw std::runtime_error("JSONDocument::init() error: cannot open file");
		}

		std::vector<u8> data = std::move(input.read());

		for (u32 i = 0; i < data.size(); i++)
		{
			if (isspace(data[i]))
			{
				if (parse_whitespace(data, i))
				{
					break;
				}
			}
			//parse string
			if(data[i] == '"')
			{
				std::string id = parse_string(data, i);

				if (isspace(data[i]))
				{
					if (parse_whitespace(data, i))
					{
						break;
					}
				}

				//string is a identifier
				if (data[i] == ':')
				{
					parse_whitespace(data, i);
					if (data[i] == '{')
					{
						m_root_named_dictionary.insert(std::pair<std::string, JSONObject*>(id, parse_dictionary(data, i)));
					}
					else if(data[i] == '"')
					{
						m_root_named_dictionary.insert(std::pair<std::string, JSONObject*>(id, new JSONObject(parse_string(data, i))));
						i--;
					}
					else if ((data[i] >= '0' && data[i] <= '9') || data[i] == '+' || data[i] == '-')
					{
						m_root_named_dictionary.insert(std::pair<std::string, JSONObject*>(id, new JSONObject(parse_number(data, i))));
						i--;
					}
					else
					{
						//TODO: error
						throw std::runtime_error("JSONDocument::init() error: unexpected symbol");
					}
				}
				else
				{
					//null
					m_root_named_dictionary.insert(std::pair<std::string, JSONObject*>(id, new JSONObject()));
				}
			}
			//parse anonymous object
			else if (data[i] == '{')
			{
				m_root_anonymous_dictionary.push_back(parse_dictionary(data, i));
			}
			else if (data[i] == ',')
			{
				continue;
			}
			else
			{
				//TODO: error
				throw std::runtime_error("JSONDocument::init() error: unexpected symbol");
			}
		}

		input.close();
	}

	/**
	 * delete the object tree structure
	 */
	void JSONDocument::clear()
	{
		for (auto* o : m_root_anonymous_dictionary)
		{
			delete o;
		}
		for (auto& it : m_root_named_dictionary)
		{
			delete it.second;
		}
	}

	/**
	 * write the modified document into a file
	 */
	void JSONDocument::write(const std::string& file_path)
	{
		File output(System::getFullPath(file_path), File::OpenMode::Write);

		if (output.failed())
		{
			throw std::runtime_error("JSONDocument::write() error: cannot open output file");
		}

		std::function<void(const char*, JSONObject*)> write_object = [&](const char* name, JSONObject* object)
		{
			if (name != nullptr)
			{
				output.write("\"" + std::string(name) + "\":");
			}

			switch (object->type())
			{
				case JSONObject::Type::Null:   { output.write("null");                                break; }
				case JSONObject::Type::Number: { output.write(std::to_string(object->number_data())); break; }
				case JSONObject::Type::String: { output.write('"' + object->string_data() + '"');     break; }
				case JSONObject::Type::Dictionary:
				{
					output.write("{");
					for (auto* o : object->anon_dict())
					{
						write_object(nullptr, o);
						output.write(",");
					}
					for (auto& it : object->named_dict())
					{
						write_object(it.first.c_str(), it.second);
						output.write(",");
					}
					output.write("}");
					break;
				}
			}
		};

		for (auto* o : m_root_anonymous_dictionary)
		{
			write_object(nullptr, o);
			if (o != m_root_anonymous_dictionary.back())
			{
				output.write(",");
			}
		}
		for (auto& it : m_root_named_dictionary)
		{
			write_object(it.first.c_str(), it.second);
			output.write(",");
		}
	}
};
