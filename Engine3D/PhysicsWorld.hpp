#pragma once

#include "Marcos.hpp"
#include "Types.hpp"

#include <vector>

#include <glm/glm.hpp>
#include "PhysicsBody.hpp"
#include "SpatialPartition.hpp"

namespace Engine3D
{
	class PhysicsWorld
	{
	public:

		PhysicsWorld() {}

		void add_global_force(glm::vec3 v);
		void add_body(PhysicsBody* body);

		void perform_collision_resolution();

	private:

		std::vector<glm::vec3>    m_global_forces;
		std::vector<PhysicsBody*> m_bodies;
		SpatialPartition          m_partition;
	};
};