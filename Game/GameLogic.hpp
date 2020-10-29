#pragma once

#include <Engine3D/Engine3D.hpp>
#include "Player.hpp"
#include "Object.hpp"
#include "Light.hpp"

#include <vector>

using u32 = Engine3D::u32;

class GameLogic final : public Engine3D::Game
{
public:

    /**
     * inherit constructors
     */
    using Engine3D::Game::Game;

    /**
     * override user functions
     */
    void init() override;
    void clean() override;
    void update(const float time_delta) override;
    void click() override;

    const glm::vec3 DefaultDiffuseColor  { 0.7f, 0.7f, 0.7f };
    const glm::vec3 DefaultAmbientColor  { 0.0f };
    const glm::vec3 DefaultSpecularColor { 0.5f };

private:
    
    std::vector<Object*>  m_objects;
    Player                m_player;
    std::vector<Engine3D::Triangle>                m_triangles;
    Engine3D::SpatialPartition<Engine3D::Triangle> m_spatial_partition;

    Light* m_light;

    Object                m_skybox;
    Engine3D::Cubemap     m_skybox_texture;
    Engine3D::Texture     m_skybox_noise;

    Engine3D::Shader      m_skybox_shader;
    Engine3D::Shader      m_obj_shader;
    Engine3D::Shader      m_post_outline_shader;
    Engine3D::Shader      m_image_shader;

    Engine3D::Music       m_main_music;
    Engine3D::Sound       m_step_left_sound;
    Engine3D::Sound       m_step_right_sound;
    Engine3D::Sound       m_jump_sound;
    Engine3D::Sound       m_land_sound;
    Engine3D::Text        m_fps;

    float                 m_time;
    float                 m_frame_time;

    bool                  m_pause { false };
};