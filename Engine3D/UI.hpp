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

#include <unordered_map>

#include "UIObject.hpp"
#include "UIPosition.hpp"

namespace Engine3D
{
	class UI
	{
	public:


		UI(u32 width, u32 height) : m_width(width), m_height(height)
		{

		}

		~UI()
		{
			for (auto& it : m_layout)
			{
				delete it.second;
			}
		}

		void      add(const std::string& name, const std::string& source,  HorizontalLayout hl = HorizontalLayout::Center, float horizontal_offset = 0, VerticalLayout vl = VerticalLayout::Center, float vertical_offset = 0);
		void      add(const std::string& name, Engine3D::UIObject* object, HorizontalLayout hl = HorizontalLayout::Center, float horizontal_offset = 0, VerticalLayout vl = VerticalLayout::Center, float vertical_offset = 0);
		void      del(const std::string& name);
		UIObject* get(const std::string& name);

		void position(const std::string& name, HorizontalLayout hl = HorizontalLayout::Center, float horizontal_offset = 0, VerticalLayout vl = VerticalLayout::Center, float vertical_offset = 0);
		void draw(Engine3D::Camera&);
		void resize(u32 width, u32 height);

	private:

		u32 m_width  { 0 };
		u32 m_height { 0 };

		std::unordered_map<std::string, UIObject*> m_layout;
	};
}