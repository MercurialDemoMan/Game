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
#include "Bullet.hpp"
#include "GameLogic.hpp"

void Bullet::update(GameLogic* game, const float delta_time)
{
	this->pos() += this->mov() * 0.5f * delta_time;

    const std::vector<Asteroid*>* asteroids = game->spatial_partition().get(this->pos());

    if (asteroids != nullptr)
    {
        for (auto& asteroid : *asteroids)
        {
            Engine3D::Collision::Data collision = Engine3D::Collision::BoxVsBox(this->pos(), this->dims() * this->scale(), asteroid->pos(), asteroid->dims() * asteroid->scale());
            /*
            Engine3D::Collision::Data collision = Engine3D::Collision::EllipsoidVsTriangle(this->pos(), this->dims(), triangle->p1, triangle->p2, triangle->p3);
            */
            if (collision)
            {
                this->destroy_flag() = true;
                asteroid->destroy_flag() = true;
                game->spawn_explosion(asteroid->pos());
                game->spatial_partition().del(asteroid);
                return;
            }
        }
    }
}