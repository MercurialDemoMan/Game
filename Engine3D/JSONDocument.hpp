#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <functional>
#include <stdexcept>

#include "File.hpp"

namespace Engine3D
{
	/**
	 * setup data types
	 */
	class JSONObject;
	using JSONNumber          = float;
	using JSONString          = std::string;
	using JSONNull            = nullptr_t;
	using JSONNamedDictionary = std::unordered_map<std::string, JSONObject*>;
	using JSONAnonDictionary  = std::vector<JSONObject*>;
	struct JSONDictionary
	{
		JSONNamedDictionary named_dict;
		JSONAnonDictionary  anon_dict;
	};
	using JSONData = std::variant<JSONNumber, JSONString, JSONNull, JSONDictionary>;


	/**
	 * object describing most json objects
	 */
	class JSONObject
	{
	public:

		/**
		 * list every type of object
		 */
		enum class Type
		{
			Number,
			String,
			Null,
			Dictionary
		};

		/**
		 * constructors for every type of object
		 */
		JSONObject() : m_type(Type::Null), m_data(JSONNull()) {}
		JSONObject(Type type) : m_type(type)
		{
			switch (type)
			{
				case Type::Number:     { m_data = JSONNumber();     break; }
				case Type::String:     { m_data = JSONString();     break; }
				case Type::Null:       { m_data = JSONNull();       break; }
				case Type::Dictionary: { m_data = JSONDictionary(); break; }
			}
		}
		JSONObject(JSONNull)        : m_type(Type::Null),   m_data(JSONNull()) {}
		JSONObject(JSONString data) : m_type(Type::String), m_data(data) {}
		JSONObject(JSONNumber data) : m_type(Type::Number), m_data(data) {}

		/**
		 * destructors
		 */
		~JSONObject()
		{	
			if(m_type == Type::Dictionary)
			{
				for (auto* o : std::get<JSONDictionary>(m_data).anon_dict)
				{
					delete o;
				}
				for (auto& it : std::get<JSONDictionary>(m_data).named_dict)
				{
					delete it.second;
				}
			}
		}

		/**
		 * access type
		 */
		const Type& type() const { return m_type; }

		/**
		 * access attributes
		 */
		JSONString&  string_data()        { return std::get<JSONString>(m_data); }
		JSONNumber&  number_data()        { return std::get<JSONNumber>(m_data); }
		JSONDictionary& dict_data()       { return std::get<JSONDictionary>(m_data); }
		JSONNamedDictionary& named_dict() { return std::get<JSONDictionary>(m_data).named_dict; }
		JSONAnonDictionary&  anon_dict()  { return std::get<JSONDictionary>(m_data).anon_dict; }

		/**
		 * iterate over named and anon dict
		 */
		void for_each_object(std::function<void(JSONObject&)> func)
		{
			for(auto* o : std::get<JSONDictionary>(m_data).anon_dict)
			{
				func(*o);
			}

			for (auto& it : std::get<JSONDictionary>(m_data).named_dict)
			{
				func(*it.second);
			}
		}
		
		/**
		 * access dictionary
		 */
		JSONObject& operator[](int i) 
		{
			if (m_type == Type::Dictionary)
			{
				return *std::get<JSONDictionary>(m_data).anon_dict[i];
			}

			throw std::runtime_error("JSONObject operator[](int) error: object is not array or dictionary");
		}
		JSONObject& operator[](std::string& i)
		{
			return this->operator[](i.c_str());
		}
		JSONObject& operator[](const char* i)
		{
			if (m_type == Type::Dictionary)
			{
				return *std::get<JSONDictionary>(m_data).named_dict[i];
			}

			throw std::runtime_error("JSONObject operator[](const char*) error: object is not dictionary");
		}

	private:

		Type        m_type { Type::Null };
		JSONData    m_data;
	};

	/**
	 * object for parsing and accessing json files
	 */
	class JSONDocument
	{
	public:

		/**
		 * constructors
		 */
		JSONDocument() {}
		JSONDocument(const std::string& file_path) { init(file_path); }
		
		/**
		 * destructor
		 */
		~JSONDocument() { clear(); }

		/**
		 * parse and load a json file
		 */
		void init(const std::string& file_path);

		/**
		 * clear memory
		 */
		void clear();

		/**
		 * write compact json file
		 */
		void write(const std::string& file_path);

		/**
		 * access root dictionaries
		 */
		JSONObject& operator[](int i)             { return *m_root_anonymous_dictionary[i]; }
		JSONObject& operator[](std::string& name) { return *m_root_named_dictionary[name]; }
		JSONObject& operator[](const char* name)  { return *m_root_named_dictionary[name]; }

	private:

		JSONAnonDictionary  m_root_anonymous_dictionary;
		JSONNamedDictionary m_root_named_dictionary;
	};
};
