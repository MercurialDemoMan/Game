/*
This file is part of Game.

Game is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Game is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Game.  If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once

#include <Engine3D/Engine3D.hpp>
#include "Player.hpp"
#include "Object.hpp"
#include "Light.hpp"
#include "Asteroid.hpp"
#include "Particle.hpp"

#include <vector>
#include <array>
#include <memory>

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
    void draw() override;
    void click() override;

    const glm::vec3 DefaultDiffuseColor  { 0.7f, 0.7f, 0.7f };
    const glm::vec3 DefaultAmbientColor  { 0.0f };
    const glm::vec3 DefaultSpecularColor { 0.5f };

    Engine3D::SpatialPartition<Asteroid>& spatial_partition() { return m_spatial_partition; }
    Player* player() { return m_player.get(); }
    void spawn_explosion(const glm::vec3& pos);
    std::vector<Object*>& objects() { return m_objects; }

private:
    
    std::vector<Object*>                 m_objects_to_insert;
    std::vector<Object*>                 m_objects;
    std::array<Light*, 20>               m_player_trail;
    std::unique_ptr<Player>              m_player;
    Engine3D::SpatialPartition<Asteroid> m_spatial_partition;

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
