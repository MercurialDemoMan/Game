/*
This file is part of Game.

Game is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Game is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Game.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "Object.hpp"

void Object::setupVertexAttributes(const Engine3D::Shader& shader)
{
    m_mesh.bindVertexPositionWithShader(shader, "in_pos");
    m_mesh.bindVertexNormalWithShader(shader, "in_nor");
    m_mesh.bindVertexUVWithShader(shader, "in_uv");
}