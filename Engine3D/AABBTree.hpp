#pragma once

#include "Shapes.hpp"
#include "Collision.hpp"

namespace Engine3D
{
	namespace Collision
	{
		class AABBObject
		{
		public:
			AABBObject()
			{

			}

			AABBObject(Engine3D::Box& box)
			{
				m_min = box.pos - box.dims / 2.0f;
				m_max = box.pos + box.dims / 2.0f;
                
                m_surface = calculate_surface();
                m_center  = calculate_center();
			}

			glm::vec3& max() { return m_max; }
			glm::vec3& min() { return m_min; }

			float surface() const
            {
                return m_surface;
            }

			void merge(const AABBObject& x, const AABBObject& y)
			{
				m_min.x = std::min(m_min.x, other.min.x);
				m_min.y = std::min(m_min.y, other.min.y);
				m_min.z = std::min(m_min.z, other.min.z);
                
				m_max.x = std::max(m_max.x, other.max.x);
				m_max.y = std::max(m_max.y, other.max.y);
				m_max.z = std::max(m_max.z, other.max.z);
                
                m_surface = calculate_surface();
                m_center  = calculate_center();
			}
            
            bool contains(const AABBObject& aabb) const
            {
                if(aabb.min().x < m_min.x) { return false; }
                if(aabb.min().y < m_min.y) { return false; }
                if(aabb.min().z < m_min.z) { return false; }
                
                if(aabb.max().x > m_min.x) { return false; }
                if(aabb.max().y > m_min.y) { return false; }
                if(aabb.max().z > m_min.z) { return false; }
            }
            
            bool overlaps(const AABBObject& aabb) const
            {
                if(aabb.max().x < m_min.x || aabb.min().x > m_max.x) { return false; }
                if(aabb.max().y < m_min.y || aabb.min().y > m_max.y) { return false; }
                if(aabb.max().z < m_min.z || aabb.min().z > m_max.z) { return false; }
                return true;
            }

		private:
        
            float calculate_surface()
			{
				glm::vec3 dims = m_max - m_min;
				return 2.0f * (dims.x * dims.y + dims.z * dims.y + dims.x * dims.z);
			}
            
            glm::vec3 calculate_center()
            {
                return m_min + (m_max - m_min) / 2.0f;
            }
        
			glm::vec3 m_min;
			glm::vec3 m_max;
            glm::vec3 m_center;
            float     m_surface;
		};

		class AABBTreeNode
		{
		public:
			AABBTreeNode() {}

			AABBObject&         data()   { return m_data; }
			u32&                parent() { return m_parent; }
            u32&                next()   { return m_next; }
			std::optional<u32>& left()   { return m_left; }
			std::optional<u32>& right()  { return m_right; }
			bool leaf()
			{
				return left.has_value();
			}

			int& height() { return m_height; }

		private:

			AABBObject         m_data;
			u32                m_parent;
            u32                m_next;
			std::optional<u32> m_left  {};
			std::optional<u32> m_right {};
			int                m_height;
		};

		class AABBTree
		{
		public:

			ABBTree()
			{

			}

			u32  add(AABBObject&   object);
			u32  add(Engine3D::Box object);
			void del(u32 id);

		private:

			u32                       m_root;
			std::vector<AABBTreeNode> m_data;
		};
	}
}











