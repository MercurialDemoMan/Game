#include "PhysicsBody.hpp"

namespace Engine3D
{
    Box& PhysicsBody::construct_bounding_box()
    {
        const std::vector<Vertex>* vertices = this->vertices();
        
        if(vertices == nullptr)
        {
            return;
        }
        
        float max_delta_sqr = 0;
        for(u32 i = 0; i < vertices.size(); i++)
        {
            glm::vec3 delta = this->getVertex(i) - this->pos();
            float delta_sqr = glm::dot(delta, delta);
            
            if(delta_sqr > max_delta)
            {
                max_delta_sqr = delta_sqr;
            }
        }
        
        
        
        return m_bounding_box;
    }
}