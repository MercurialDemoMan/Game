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
#include "UI.hpp"

namespace Engine3D
{
	void UI::add(const std::string& name, const std::string& source, HorizontalLayout hl, float horizontal_offset, VerticalLayout vl, float vertical_offset)
	{
		m_layout[name] = new UIObject(glm::vec3(0), source.c_str());
		this->position(name, hl, horizontal_offset, vl, vertical_offset);
	}
	void UI::add(const std::string& name, Engine3D::UIObject* object, HorizontalLayout hl, float horizontal_offset, VerticalLayout vl, float vertical_offset)
	{
		m_layout[name] = object;
		this->position(name, hl, horizontal_offset, vl, vertical_offset);
	}

	void UI::del(const std::string& name)
	{
		m_layout.erase(name);
	}
	UIObject* UI::get(const std::string& name)
	{
		auto it = m_layout.find(name);
		if (it == m_layout.end())
		{
			return nullptr;
		}
		return it->second;
	}
	void UI::position(const std::string& name, HorizontalLayout hl, float horizontal_offset, VerticalLayout vl, float vertical_offset)
	{
		auto it = m_layout.find(name);
		if (it != m_layout.end())
		{
			UIObject* sprite = it->second;
			if (sprite != nullptr)
			{
				sprite->ui_position() = { hl, horizontal_offset, vl, vertical_offset };

				if (hl == HorizontalLayout::Left)
				{
					sprite->pos().x = -(m_width / 4.0f) + (sprite->dims().x) + horizontal_offset * 2.0f;
				}
				else if (hl == HorizontalLayout::Right)
				{
					sprite->pos().x = +(m_width / 4.0f) - (sprite->dims().x) - horizontal_offset * 2.0f;
				}
				else
				{
					sprite->pos().x = horizontal_offset * 2.0f;
				}

				if (vl == VerticalLayout::Bot)
				{
					sprite->pos().y = -(m_height / 4.0f) + (sprite->dims().y) + vertical_offset * 2.0f;
				}
				else if (vl == VerticalLayout::Top)
				{
					sprite->pos().y = +(m_height / 4.0f) - (sprite->dims().y) - vertical_offset * 2.0f;
				}
				else
				{
					sprite->pos().y = vertical_offset * 2.0f;
				}
			}
		}
	}
	void UI::draw(Engine3D::Camera& cam)
	{
		for (auto& object : m_layout)
		{
			object.second->draw(cam);
		}
	}

	void UI::resize(u32 width, u32 height)
	{
		m_width  = width;
		m_height = height;

		for (auto object : m_layout)
		{
			this->position(object.first, object.second->ui_position().horizontal_layout, object.second->ui_position().horizontal_offset,
										 object.second->ui_position().vertical_layout,   object.second->ui_position().vertical_offset);
		}
	}
}