#pragma once

#include <Engine3D/Camera.hpp>
#include <Engine3D/Types.hpp>

/**
 * manage player
 */
class Player
{
public:
	
	/**
	 * constructors
	 */
	Player() {}
	
	/**
	 * constants
	 */
	constexpr static float Gravity            = 0.008f; //0.008
	constexpr static float JumpPower          = 0.375f; //0.375
	constexpr static float Acceleration       = 0.01f;
	constexpr static float Decceleration      = 0.15f;
	constexpr static float MaxHorizontalSpeed = 0.5f;
	constexpr static float MaxVerticalSpeed   = 5.0f;
	constexpr static float LandBobThreshold   = -0.25f;
	constexpr static float LandBob            = -0.75f;
	constexpr static Engine3D::u32 CoyoteJumpDefault = 20;

	/**
	 * getters
	 */
	const glm::vec3&  getPos() { return m_pos; }
	const glm::vec3&  getDir() { return m_camera.getDir(); }
	Engine3D::Camera& getCam() { return m_camera; }
	/**
	 * setters
	 */
	void setPos(glm::vec3& pos)  { m_pos = pos; m_camera.setPos(m_pos + glm::vec3(0, m_bobbing + m_land_bob, 0)); }
	void setPos(glm::vec3&& pos) { m_pos = pos; m_camera.setPos(m_pos + glm::vec3(0, m_bobbing + m_land_bob, 0)); }
	void addPos(glm::vec3& pos)  { m_pos += pos; m_camera.setPos(m_pos + glm::vec3(0, m_bobbing + m_land_bob, 0)); }
	void addPos(glm::vec3&& pos) { m_pos += pos; m_camera.setPos(m_pos + glm::vec3(0, m_bobbing + m_land_bob, 0)); }
	void setDir(glm::vec3& dir)  { m_camera.setDir(dir); }
	void setDir(glm::vec3&& dir) { m_camera.setDir(dir); }

	/**
	 * access attributes
	 */
	glm::vec3& mov()             { return m_mov; }
	glm::vec3& dims()            { return m_dims; }
	float&     bobbing()         { return m_bobbing; }
	float&     land_bob()        { return m_land_bob; }
	float&     coyote_jump()     { return m_coyote_jump; }
	glm::vec3& on_ground_slope() { return m_on_ground_slope; }
	bool&      on_ground()       { return m_on_ground; }
	bool&      can_jump()        { return m_can_jump; }
	bool&      step()            { return m_step; }

	void reset()
	{
		setPos(glm::vec3(0));
		setDir(glm::vec3(0, 0, 1));
		mov() = glm::vec3(0);
		dims() = glm::vec3(1, 3.3, 1);
		bobbing() = 0;
		land_bob() = 0;
		coyote_jump() = 0;
		on_ground_slope() = glm::vec3(0, 1, 0);
		on_ground() = false;
		can_jump() = false;
		step() = 0;
	}


private:
	Engine3D::Camera m_camera;
	glm::vec3        m_pos         { 0 };
	glm::vec3        m_mov         { 0 };
	glm::vec3        m_dims        { 1, 3.3, 1 };
	float            m_bobbing     { 0 };
	float            m_land_bob    { 0 };
	float            m_coyote_jump { 0 };

	glm::vec3        m_on_ground_slope { 0, 1, 0 };
	bool             m_on_ground       { false };
	bool             m_can_jump        { false };
	bool             m_step            { false };
};