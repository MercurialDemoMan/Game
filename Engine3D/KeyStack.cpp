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
#include "KeyStack.hpp"

#include <algorithm>

namespace Engine3D
{
	void KeyStack::update(Game* game)
	{
		for (auto& key : m_keys_to_check)
		{
			if (game->keyDown(key.key) && !key.pressed)
			{
				m_key_stack.push_back(key.key);
				key.pressed = true;
			}
			if (!game->keyDown(key.key) && key.pressed)
			{
				m_key_stack.erase(std::partition(m_key_stack.begin(), m_key_stack.end(), [&](const auto& obj)
					{
						return obj != key.key;
					}), m_key_stack.end());
				key.pressed = false;
			}
		}
	}

	std::optional<Game::Key> KeyStack::pressed()
	{
		if (m_key_stack.size() == 0)
		{
			return {};
		}

		return m_key_stack.back();
	}

	bool KeyStack::keyDown(Game::Key key)
	{
		std::optional<Game::Key> pressed = this->pressed();

		if (pressed)
		{
			return pressed.value() == key;
		}

		return false;
	}
}