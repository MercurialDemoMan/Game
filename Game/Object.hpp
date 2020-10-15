#pragma once

#include "Engine3D/Shader.hpp"
#include "Engine3D/SceneObject.hpp"

#include <functional>

class Object final : public Engine3D::SceneObject
{
public:

	enum class HitboxType
	{
		None,
		Box,
		Cylinder,
		Ball,
		Mesh
	};

	/**
	 * constructors
	 */
	Object() : SceneObject() {}
	Object(glm::vec3 pos) : SceneObject(pos) {}
	Object(glm::vec3 pos, const char* mesh_id) : SceneObject(pos, mesh_id)
	{
		
	}
	Object(glm::vec3 pos, const std::string& mesh_id) : SceneObject(pos, mesh_id)
	{
		
	}
	Object(glm::vec3 pos, const char* mesh_id, HitboxType hitbox) : SceneObject(pos, mesh_id), m_hitbox_type(hitbox)
	{

	}
	Object(glm::vec3 pos, const std::string& mesh_id, HitboxType hitbox) : SceneObject(pos, mesh_id), m_hitbox_type(hitbox)
	{

	}
	Object(glm::vec3 pos, const char* mesh_id, HitboxType hitbox, std::function<void(void)> collision_trigger) : SceneObject(pos, mesh_id), m_hitbox_type(hitbox), m_collision_trigger(collision_trigger)
	{

	}
	Object(glm::vec3 pos, const std::string& mesh_id, HitboxType hitbox, std::function<void(void)> collision_trigger) :SceneObject(pos, mesh_id), m_hitbox_type(hitbox), m_collision_trigger(collision_trigger)
	{

	}

	/**
	 * access attributes
	 */
	HitboxType&               hitbox()                 { return m_hitbox_type; }
	std::function<void(void)> collision_trigger()      { return m_collision_trigger; }
	bool&                     has_collision_response() { return m_has_collision_response; }

	void setupVertexAttributes(const Engine3D::Shader& shader);

private:

	HitboxType                m_hitbox_type { HitboxType::None };
	std::function<void(void)> m_collision_trigger;
	bool                      m_has_collision_response { true };
};