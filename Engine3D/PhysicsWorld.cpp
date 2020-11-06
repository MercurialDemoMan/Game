#include "PhysicsWorld.hpp"


void PhysicsWorld::add_global_force(glm::vec3 v)
{
	m_global_forces.push_back(v);
}
void PhysicsWorld::add_body(std::shared_ptr<PhysicsBody> body)
{
	m_bodies.push_back(body);
}

void PhysicsWorld::perform_collision_resolution()
{
    
}
