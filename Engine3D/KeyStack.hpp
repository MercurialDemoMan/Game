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
#include "Game.hpp"

#include <vector>
#include <optional>

namespace Engine3D
{
	/**
	 * mutiple key presses manager
	 */
	class KeyStack
	{
	public:

		/**
		 * main constructor 
		 */
		template<typename... T>
		KeyStack(T&&... args) : m_keys_to_check { args... }
		{
			
		}

		/**
		 * scan for key presses
		 */
		void update(Game*);

		/**
		 * get the correct key press 
		 */
		std::optional<Game::Key> pressed();
		bool keyDown(Game::Key);

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

		std::vector<Key>       m_keys_to_check;
		std::vector<Game::Key> m_key_stack;
	};
};