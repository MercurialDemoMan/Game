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
#include "UIObject.hpp"

namespace Engine3D
{
	void UIObject::draw(Engine3D::Camera& cam)
	{
		this->pos().x += cam.getPos().x;
		this->pos().y += cam.getPos().y;
		this->pos().z = cam.getPos().z + 1;
		Engine3D::Sprite::draw(cam);
		this->pos().x -= cam.getPos().x;
		this->pos().y -= cam.getPos().y;
	}
}