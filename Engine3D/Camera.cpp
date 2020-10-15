#include "Camera.hpp"

#include <glm/gtx/rotate_vector.hpp>

namespace Engine3D
{
	/**
	 * rotate the camera around a axis
	 */
	void Camera::rotate(glm::vec3& axis, float angle_in_rad)
	{
		m_rot_needs_update = m_view_needs_update = true;
		m_dir = glm::rotate(m_dir, angle_in_rad, axis);
	}
	void Camera::rotate(glm::vec3&& axis, float angle_in_rad)
	{
		m_rot_needs_update = m_view_needs_update = true;
		m_dir = glm::rotate(m_dir, angle_in_rad, axis);
	}

	/**
	 * calculate the rotation matrix
	 */
	const glm::mat4& Camera::getRotationMatrix()
	{
		if(m_rot_needs_update)
		{
			m_rot_needs_update = false;
			
			glm::vec3 x_axis = -glm::normalize(glm::cross(m_dir, glm::vec3(0.0f, 1.0f, 0.0f)));
			glm::vec3 y_axis =  glm::normalize(glm::cross(x_axis, m_dir));
			
			m_rot_matrix =
			{
				{ x_axis.x, x_axis.y, x_axis.z, 0.0f },
				{ y_axis.x, y_axis.y, y_axis.z, 0.0f },
				{  m_dir.x,  m_dir.y,  m_dir.z, 0.0f },
				{     0.0f,     0.0f,     0.0f, 1.0f }
			};
			
			return const_cast<const glm::mat4&>(m_rot_matrix);
		}
		else
		{
			return const_cast<const glm::mat4&>(m_rot_matrix);
		}
	}
	
	/**
	 * calculate the view matrix
	 */
	const glm::mat4& Camera::getViewMatrix()
	{
		if(m_view_needs_update)
		{
			m_view_needs_update = false;
			
			static glm::vec3 up = glm::vec3(0, 1, 0);

			glm::vec3 right  = glm::normalize(glm::cross(getDir(), up));
			glm::vec3 upward = glm::normalize(glm::cross(right, getDir()));

			m_view_matrix =
			{
				{ right.x, upward.x, -getDir().x, 0 },
			    { right.y, upward.y, -getDir().y, 0 },
			    { right.z, upward.z, -getDir().z, 0 },
			    { -glm::dot(right, getPos()), -glm::dot(upward, getPos()), glm::dot(getDir(), getPos()), 1 }
			};
			
			return const_cast<const glm::mat4&>(m_view_matrix);
		}
		else
		{
			return const_cast<const glm::mat4&>(m_view_matrix);
		}
	}
};