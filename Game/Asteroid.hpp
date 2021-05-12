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

class Asteroid : public Object
{
public:
	explicit Asteroid(const glm::vec3& pos, const glm::vec3& mov, const char* model = "data/objects/rock.obj") : Object(pos, model)
	{
		this->mov() = mov;
		this->scale() *= 0.2;
        m_random_rotation = glm::normalize(glm::vec3(
            Engine3D::Random::uniform(0.1, 1),
            Engine3D::Random::uniform(0.1, 1),
            Engine3D::Random::uniform(0.1, 1)
        ));

        m_spatial_partition.init(Engine3D::SpatialPartition<Engine3D::Triangle>::DefaultCellSize, [this](const Engine3D::Triangle& triangle) -> Engine3D::BoundingBox
            {
                Engine3D::BoundingBox r;

                r.min.x = std::min(triangle.p1.x, std::min(triangle.p2.x, triangle.p3.x));
                r.min.y = std::min(triangle.p1.y, std::min(triangle.p2.y, triangle.p3.y));
                r.min.z = std::min(triangle.p1.z, std::min(triangle.p2.z, triangle.p3.z));
                                                                                            
                r.max.x = std::max(triangle.p1.x, std::max(triangle.p2.x, triangle.p3.x));
                r.max.y = std::max(triangle.p1.y, std::max(triangle.p2.y, triangle.p3.y));
                r.max.z = std::max(triangle.p1.z, std::max(triangle.p2.z, triangle.p3.z));

                return r;
            });

        std::vector<Engine3D::Triangle> triangles {};
        const std::vector<Engine3D::Vertex>* vertices = this->vertices();

        if (vertices->size() != 0)
        {
            for (Engine3D::u32 i = 0; i < vertices->size(); i += 3)
            {
                triangles.push_back(this->constructTriangle(i));
            }
        }

        m_spatial_partition.update(triangles);
	}

	virtual void update(GameLogic*, const float delta_time) override;

	Engine3D::SpatialPartition<Engine3D::Triangle>& spatial_partition() { return m_spatial_partition; }

private:

	Engine3D::SpatialPartition<Engine3D::Triangle> m_spatial_partition;
    glm::vec3 m_random_rotation;
};