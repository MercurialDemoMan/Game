#pragma once

#include "SceneObject.hpp"
#inlcude "Shapes.hpp"

namespace Engine3D
{
	class PhysicsBody : public SceneObject
	{
	public:
		ENGINE3D_INHERIT_CONSTRUCTORS(SceneObject, SceneObject);

		void add_collision_function(std::function<void(PhysicsBody*)> func) : m_collision_function(func) {}
        Box& construct_bounding_box();

	private:

		std::function<void(PhysicsBody*)> m_collision_function { nullptr };
        Box                               m_bounding_box;
	};
};