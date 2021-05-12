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
#include "Asteroid.hpp"
#include "GameLogic.hpp"

void Asteroid::update(GameLogic* game, const float delta_time)
{
	this->rotate(0.01 * delta_time, m_random_rotation);

    /*
	auto objects = game->spatial_partition().get(this->pos());

    if (objects != nullptr)
    {
        for (auto& asteroid : *objects)
        {
            Engine3D::Collision::Data collision = Engine3D::Collision::BallVsBall(this->pos(), this->dims().x, asteroid->pos(), asteroid->dims().x);
            this->pos() += collision.displacement;
        }
    }
    */
}