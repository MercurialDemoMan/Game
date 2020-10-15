#pragma once

#include "Engine3D/BillboardObject.hpp"

#include <glm/glm.hpp>

class Light final : public Engine3D::BillboardObject
{
public:

	/**
	 * constructors
	 */
	Light() : BillboardObject() {}
	Light(glm::vec3 pos) : BillboardObject(pos) {}
	Light(glm::vec3 pos, const char* tex_id) : BillboardObject(pos, tex_id)
	{
	}
	Light(glm::vec3 pos, const std::string& tex_id) : BillboardObject(pos, tex_id)
	{
	}

	glm::vec3& ambient_color()  { return m_ambient_color; }
	glm::vec3& diffuse_color()  { return m_diffuse_color; }
	glm::vec3& specular_color() { return m_specular_color; }

private:

	glm::vec3 m_ambient_color  { 0.2f };
	glm::vec3 m_diffuse_color  { 1.0f };
	glm::vec3 m_specular_color { 1.0f };
};