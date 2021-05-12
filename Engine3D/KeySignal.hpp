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

#include "Game.hpp"
#include <optional>
#include <functional>
#include <vector>

namespace Engine3D
{
	/**
	 * capture key press and release 
	 */
	class KeySignal
	{
	public:

		/**
		 * main constructor
		 */
		template<typename... T>
		KeySignal(std::function<void(Game::Key)> press_callback, std::function<void(Game::Key)> unpress_callback, T&&... args) : m_keys_to_check { args... }
		{
			m_press_callback   = press_callback;
			m_unpress_callback = unpress_callback;
		}

		/**
		 * scan for key presses
		 */
		void update(Game*);

	private:

		/**
		 * internal key structure for press check
		 */
		struct Key
		{
			bool      pressed { false };
			Game::Key key;

			Key(Game::Key key) : key(key) { }
		};

		std::vector<Key> m_keys_to_check;

		std::function<void(Game::Key)> m_press_callback;
		std::function<void(Game::Key)> m_unpress_callback;
	};
};