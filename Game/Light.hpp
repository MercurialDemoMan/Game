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
#pragma once

#include "Engine3D/BillboardObject.hpp"

#include <glm/glm.hpp>

class Light final : public Engine3D::BillboardObject
{
public:

    /**
     * constructors
     */
    Light() : BillboardObject() {}
    Light(glm::vec3 pos) : BillboardObject(pos) {}
    Light(glm::vec3 pos, const char* tex_id) : BillboardObject(pos, tex_id)
    {
    }
    Light(glm::vec3 pos, const std::string& tex_id) : BillboardObject(pos, tex_id)
    {
    }

    glm::vec3& ambient_color()  { return m_ambient_color; }
    glm::vec3& diffuse_color()  { return m_diffuse_color; }
    glm::vec3& specular_color() { return m_specular_color; }

private:

    glm::vec3 m_ambient_color  { 0.2f };
    glm::vec3 m_diffuse_color  { 1.0f };
    glm::vec3 m_specular_color { 1.0f };
};