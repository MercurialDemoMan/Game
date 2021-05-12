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

#include <Engine3D/Engine3D.hpp>
#include "Object.hpp"

class Particle : public Object
{
public:
	static constexpr const float DefaultLifeTime = 120;

	Particle(const glm::vec3& pos, const glm::vec3& mov) : 
		Object(pos, "data/objects/cube.obj")
	{
		this->mov() = mov;
		this->scale() *= Engine3D::Random::uniform(0.3, 0.5);
		m_life = DefaultLifeTime;
	}

	virtual void update(GameLogic*, const float delta_time) override;

private:

	float m_life {};
};