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

#include "Sprite.hpp"
#include "UIPosition.hpp"

namespace Engine3D
{
	class UIObject : public Engine3D::Sprite
	{
	public:
		ENGINE3D_INHERIT_CONSTRUCTORS(Engine3D::Sprite, Sprite);

		virtual void draw(Engine3D::Camera&) override;

		UIPosition& ui_position() { return m_ui_position; }

	private:

		UIPosition m_ui_position;
	};
}