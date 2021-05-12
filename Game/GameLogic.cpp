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
#include "GameLogic.hpp"
#include "Bullet.hpp"

#include <iostream>
#include <algorithm>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

/**
 * Engine3D callback functions
 */
void GameLogic::init()
{

    this->setVSync(Engine3D::Game::VSync::On);

    this->setBackgroundColor(0.05f, 0.05f, 0.2f);

    this->setMouseAnchor(this->getDims() / 2.f);
    
    //initialize spatial partition for static triangles
    m_spatial_partition.init(Engine3D::SpatialPartition<Asteroid>::DefaultCellSize, [this](const Asteroid& a) -> Engine3D::BoundingBox
        {
            Engine3D::BoundingBox r;

            r.min = a.get_pos() - (a.get_dims() * a.get_scale());
            r.max = a.get_pos() + (a.get_dims() * a.get_scale());

            return r;
        });
        
    //load shaders
    Engine3D::inline_try<std::runtime_error>([&]
        {
            m_obj_shader.init("data/shaders/objects.vert", "data/shaders/objects.frag");
            m_image_shader.init("data/shaders/image.vert", "data/shaders/image.frag");
            m_skybox_shader.init("data/shaders/skybox.vert", "data/shaders/skybox_clouds.frag");
            m_post_outline_shader.init("data/shaders/scene_post.vert", "data/shaders/scene_post_outline.frag");
        }, "GameLogic::Engine3D_init()");
    
    //load lights
    Engine3D::inline_try<std::runtime_error>([&]
        {
            m_light = new Light(glm::vec3(0), "data/textures/light.png");
            m_light->scale() *= 0.001;
            m_light->diffuse_color() = m_light->specular_color() = glm::vec3(0.9, 0.1, 0.5);

            for (u32 i = 0; i < m_player_trail.size(); i++)
            {
                m_player_trail[i] = new Light(glm::vec3(0), "data/textures/light.png");
                m_player_trail[i]->scale() *= 0.001;
                m_player_trail[i]->diffuse_color() = m_light->specular_color() = glm::vec3(0.9, 0.1, 0.5);
            }
        }, "GameLogic::Engine3D_init()");

    //load textures
    Engine3D::inline_try<std::runtime_error>([&]
        {
            m_skybox_texture.init("data/textures/skybox");
        }, "GameLogic::Engine3D_init()");
    Engine3D::inline_try<std::runtime_error>([&]
        {
            m_skybox_noise.init("data/textures/noise256.png");
        }, "GameLogic::Engine3D_init()");

    //TODO: load level
    

    //load skybox
    Engine3D::inline_try<std::runtime_error>([&]
        {
            m_skybox.init(glm::vec3(0), "data/objects/inv_cube.obj");
            m_skybox.scale() = glm::vec3(this->DefaultFrustrumMax / sqrtf(3));
            m_skybox.setupVertexAttributes(m_skybox_shader);
        }, "GameLogic::Engine3D_init()");
        
    //load level
    Engine3D::inline_try<std::runtime_error>([&]
        {
            const char* asteroids_models[] = {
                "data/objects/rock.obj",
                "data/objects/island.obj",
                "data/objects/cube.obj"
            };


            for (Engine3D::u32 i = 0; i < 100; i++)
            {
                u32 asteroids_model_index = u32(Engine3D::Random::uniform(0, sizeof(asteroids_models) / sizeof(asteroids_models[0])));
                m_objects.push_back(new Asteroid(glm::vec3(  0 + Engine3D::Random::uniform(-100, 100), 
                                                           -20 + Engine3D::Random::uniform(-100, 100), 
                                                           -20 + Engine3D::Random::uniform(-10, 100)), glm::vec3(0), asteroids_models[asteroids_model_index]));
                m_objects.back()->scale() = glm::vec3(2 + Engine3D::Random::uniform(0, 5));
                m_objects.back()->hitbox() = Object::HitboxType::Mesh;
m_objects.back()->setupVertexAttributes(m_obj_shader);
m_objects.back()->col() = glm::vec3(Engine3D::Random::uniform(0.2, 0.6), Engine3D::Random::uniform(0.2, 0.4), Engine3D::Random::uniform(0.0, 0.05));
            }

        }, "GameLogic::Engine3D_init()");

        //show fps text box

        Engine3D::inline_try<std::runtime_error>([&]
            {
                m_fps.init(*this, "data/fonts/GoodTimes.ttf", 25);
            }, "GameLogic::Engine3D_init()");

        Engine3D::inline_try<std::runtime_error>([&]
            {
                m_main_music.init("data/audio/abstract.ogg");
                //m_main_music.play();

                m_step_left_sound.init("data/audio/step_left.wav");
                m_step_right_sound.init("data/audio/step_right.wav");
                m_jump_sound.init("data/audio/jump.wav");
                m_land_sound.init("data/audio/land_soft.wav");
            }, "GameLogic::Engine3D_init()");

        m_player = std::make_unique<Player>(m_obj_shader);
        m_player->reset();

        std::printf("GameLogic() log: constructing spatial partitions...\n");

        //construct spatial partition

        std::vector<Asteroid*> asteroids;

        for (auto& object : m_objects)
        {
            if (object->hitbox() != Object::HitboxType::Mesh)
            {
                continue;
            }

            asteroids.push_back(reinterpret_cast<Asteroid*>(object));
        }

        m_spatial_partition.update(asteroids);

        std::printf("GameLogic() log: spatial partitions constructed\n");
        std::printf("--------------------\n");
        std::printf("Controls: W, A, S, D\n");
        std::printf("Arrow Up, Arrow Down\n\n");
        std::printf("Shooting: Space\n");
        std::printf("--------------------\n");
}

void GameLogic::clean()
{
    this->freeMouse();

    for (auto& object : m_objects)
    {
        delete object;
    }

    delete m_light;

    for (auto& light : m_player_trail)
    {
        delete light;
    }

    m_main_music.stop();
}

void GameLogic::update(const float time_delta)
{
    if (m_pause == false)
    {
        m_time += (float)fmod(1.0f * time_delta, M_PI * 2.0);

        glm::vec3 dir;
        if (glm::dot(m_player->mov(), m_player->mov()) == 0)
        {
            dir = glm::normalize(m_player->getDir());
        }
        else
        {
            dir = glm::normalize(m_player->mov());
        }
        glm::vec3 rig = glm::cross(glm::vec3(0, 1, 0), m_player->mov());
        glm::vec3 up = glm::cross(rig, m_player->mov());

        if (this->keyDown(Engine3D::Game::Key::W) && !this->keyDown(Engine3D::Game::Key::S))
        {
            m_player->speed() += Player::Acceleration * time_delta;
            //m_player->mov() += dir * Player::Acceleration * time_delta;
        }

        if (this->keyDown(Engine3D::Game::Key::S) && !this->keyDown(Engine3D::Game::Key::W))
        {
            m_player->speed() -= Player::Acceleration * time_delta;
            if (m_player->speed() < 0)
            {
                m_player->speed() = 0;
            }
            //m_player->mov() -= dir * Player::Acceleration * time_delta;
        }

        m_player->mov() = dir * m_player->speed();

        if (this->keyDown(Engine3D::Game::Key::A) && !this->keyDown(Engine3D::Game::Key::D))
        {
            m_player->mov() = glm::rotate(m_player->mov(), -0.05f * time_delta, glm::vec3(0, 1, 0));
            m_player->setDir(glm::rotate(m_player->getDir(), -0.05f * time_delta, glm::vec3(0, 1, 0)));
            m_player->rotate(-0.05f * time_delta, glm::vec3(0, 1, 0));
        }

        if (this->keyDown(Engine3D::Game::Key::D) && !this->keyDown(Engine3D::Game::Key::A))
        {
            m_player->mov() = glm::rotate(m_player->mov(), +0.05f * time_delta, glm::vec3(0, 1, 0));
            m_player->setDir(glm::rotate(m_player->getDir(), +0.05f * time_delta, glm::vec3(0, 1, 0)));
            m_player->rotate(+0.05f * time_delta, glm::vec3(0, 1, 0));
        }

        if (this->keyDown(Engine3D::Game::Key::UP) && !this->keyDown(Engine3D::Game::Key::DOWN))
        {
            m_player->pos() += glm::vec3(0, 0.1f, 0) * time_delta;
        }
        if (this->keyDown(Engine3D::Game::Key::DOWN) && !this->keyDown(Engine3D::Game::Key::UP))
        {
            m_player->pos() -= glm::vec3(0, 0.1f, 0) * time_delta;
        }

        if (m_player->gun_reload() > 0)
        {
            m_player->gun_reload() -= 1 * time_delta;
        }
        if (this->keyDown(Engine3D::Game::Key::SPACE) && m_player->gun_reload() <= 0)
        {
            m_objects.push_back(new Bullet(m_player->pos(), m_player->getDir()));
            m_objects.back()->setupVertexAttributes(m_obj_shader);
            m_player->gun_reload() = Player::DefaultReload;
        }

        m_player->pos() += m_player->mov() * time_delta;
        
        glm::vec3 collision_result(0);

        //perform collision with static objects

        auto asteroids = m_spatial_partition.get(m_player->pos());

        if (asteroids != nullptr)
        {
            for (auto& asteroid : *asteroids)
            {
                auto triangles = asteroid->triangles();

                for (auto& triangle : triangles)
                {
                    Engine3D::Collision::Data collision = Engine3D::Collision::EllipsoidVsTriangle(m_player->pos(), m_player->dims(), triangle.p1, triangle.p2, triangle.p3);
                    m_player->pos() += collision.displacement;
                    collision_result += collision.displacement;
                }
            }
        }

        if (m_objects_to_insert.size() != 0)
        {
            m_objects.insert(m_objects.end(), m_objects_to_insert.begin(), m_objects_to_insert.end());
        }
        m_objects_to_insert.clear();

        for (auto object : m_objects)
        {
            if (object->destroy_flag())
            {
                continue;
            }

            object->update(this, time_delta);
        }

        //remove objects whom requested it
        m_objects.erase(std::partition(m_objects.begin(), m_objects.end(), [](const auto& obj)
            {
                if (obj->destroy_flag())
                {
                    delete obj;
                    return false;
                }
                return true;
            }), m_objects.end());
            
        
        static u32 cicler = 0;

        //rotate camera around its right vector + gimbal lock the rotation to only 180 deg
        if (glm::dot(m_player->mov(), m_player->mov()) == 0)
        {
            glm::vec3 dir = glm::normalize(m_player->getDir());
            m_player->getCam().setDir(dir);
            m_player->getCam().setPos(m_player->pos() - (dir * 2.0f) + glm::vec3(0, 1, 0));
            m_light->pos() = m_player->pos() - (dir / 2.0f);
            m_player_trail[cicler]->pos() = m_player->pos() - (dir / 2.0f);

        }
        else
        {
            glm::vec3 dir = glm::normalize(m_player->mov());
            m_player->getCam().setDir(dir);
            m_player->getCam().setPos(m_player->pos() - (dir * 2.0f) + glm::vec3(0, 1, 0));
            m_light->pos() = m_player->pos() - (dir / 2.0f);
            m_player_trail[cicler]->pos() = m_player->pos() - (dir / 2.0f);
        }
        

        //set fov based on how fast are we moving
        m_player->getCam().setFov(m_player->getCam().getFov() + (100.0f + (sqrt(pow(m_player->mov().x, 2.0f) + pow(m_player->mov().z, 2.0f)) * 50.0f) - m_player->getCam().getFov()) / 10.0f);

        cicler = (cicler + 1) % m_player_trail.size();

        //update skybox position
        m_skybox.pos() = m_player->pos();
    }
}

void GameLogic::draw()
{
    
    this->drawBegin3D(m_player->getCam());
    
    //draw skybox
    m_skybox_shader.use();

    m_skybox_shader.setTextureCubemap(m_skybox_texture.getID(), "skybox_texture");
    m_skybox_shader.setTexture2D(m_skybox_noise.getID(), "noise_texture");
    m_skybox_shader.set4x4m(m_player->getCam().getRotationMatrix(), "rotation_matrix");
    m_skybox_shader.set3f(glm::vec3(0.5, 0.5, 1), "color");
    m_skybox_shader.set1f(m_time / 100.0f, "time");

    m_skybox.draw();

    m_skybox_shader.unuse();

    //draw static object

    for (auto& object : m_objects)
    {
        m_obj_shader.use();

        m_obj_shader.setTextureCubemap(m_skybox_texture.getID(), "skybox");

        const Engine3D::Material* material = object->material();

        if (material != nullptr)
        {
            if (const_cast<Engine3D::Material*>(material)->diffuse_mapping_texture.empty() == false)
            {
                m_obj_shader.setTexture2D(const_cast<Engine3D::Material*>(object->material())->diffuse_mapping_texture.getID(), "uv_map");
                m_obj_shader.set1b(true, "has_uv_map");
            }
            else
            {
                m_obj_shader.setTexture2D(m_skybox_noise.getID(), "uv_map");
                m_obj_shader.set1b(false, "has_uv_map");
            }

            m_obj_shader.set3f(object->material()->ambient * object->col(), "material_ambient");
            m_obj_shader.set3f(object->material()->diffuse * object->col(), "material_diffuse");
            m_obj_shader.set3f(object->material()->specular * object->col(), "material_specular");
        }
        else
        {
            m_obj_shader.set3f(DefaultAmbientColor, "material_ambient");
            m_obj_shader.set3f(DefaultDiffuseColor, "material_diffuse");
            m_obj_shader.set3f(DefaultSpecularColor, "material_specular");
            m_obj_shader.setTexture2D(m_skybox_noise.getID(), "uv_map");
            m_obj_shader.set1b(false, "has_uv_map");
        }

        m_obj_shader.set3f(m_light->ambient_color(), "light_ambient");
        m_obj_shader.set3f(m_light->diffuse_color(), "light_diffuse");
        m_obj_shader.set3f(m_light->specular_color(), "light_specular");
        m_obj_shader.set1f(4, "material_shininess");
        m_obj_shader.set2f(this->getDims(), "dims");
        m_obj_shader.set1f(1, "reflection_strength");
        m_obj_shader.set3f(m_light->pos(), "light_pos");
        m_obj_shader.set4x4m(m_player->getCam().getRotationMatrix(), "rot_mat");
        m_obj_shader.set4x4m(m_player->getCam().getViewMatrix(), "view_mat");
        m_obj_shader.set1f(m_time / 100.0f, "time");
        m_obj_shader.set3f(object->pos(), "obj_pos");

        object->draw();

        m_obj_shader.unuse();
    }

    m_obj_shader.use();

    m_obj_shader.setTextureCubemap(m_skybox_texture.getID(), "skybox");

    const Engine3D::Material* material = m_player->material();

    if (material != nullptr)
    {
        if (const_cast<Engine3D::Material*>(material)->diffuse_mapping_texture.empty() == false)
        {
            m_obj_shader.setTexture2D(const_cast<Engine3D::Material*>(m_player->material())->diffuse_mapping_texture.getID(), "uv_map");
            m_obj_shader.set1b(true, "has_uv_map");
        }
        else
        {
            m_obj_shader.setTexture2D(m_skybox_noise.getID(), "uv_map");
            m_obj_shader.set1b(false, "has_uv_map");
        }

        m_obj_shader.set3f(m_player->material()->ambient * m_player->col(), "material_ambient");
        m_obj_shader.set3f(m_player->material()->diffuse * m_player->col(), "material_diffuse");
        m_obj_shader.set3f(m_player->material()->specular * m_player->col(), "material_specular");
    }
    else
    {
        m_obj_shader.set3f(DefaultAmbientColor, "material_ambient");
        m_obj_shader.set3f(DefaultDiffuseColor, "material_diffuse");
        m_obj_shader.set3f(DefaultSpecularColor, "material_specular");
        m_obj_shader.setTexture2D(m_skybox_noise.getID(), "uv_map");
        m_obj_shader.set1b(false, "has_uv_map");
    }

    m_obj_shader.set3f(m_light->ambient_color(), "light_ambient");
    m_obj_shader.set3f(m_light->diffuse_color(), "light_diffuse");
    m_obj_shader.set3f(m_light->specular_color(), "light_specular");
    m_obj_shader.set1f(4, "material_shininess");
    m_obj_shader.set2f(this->getDims(), "dims");
    m_obj_shader.set1f(1, "reflection_strength");
    m_obj_shader.set3f(m_light->pos(), "light_pos");
    m_obj_shader.set4x4m(m_player->getCam().getRotationMatrix(), "rot_mat");
    m_obj_shader.set4x4m(m_player->getCam().getViewMatrix(), "view_mat");
    m_obj_shader.set1f(m_time / 100.0f, "time");
    m_obj_shader.set3f(m_player->pos(), "obj_pos");

    m_player->draw();

    m_obj_shader.unuse();

    m_image_shader.use();
    m_image_shader.setTexture2D(m_light->texture().getID(), "image");
    m_image_shader.set3f(m_light->diffuse_color(), "color");

    m_light->draw(m_player->getCam());
    for (auto& light : m_player_trail)
    {
        light->draw(m_player->getCam());
    }

    m_image_shader.unuse();

    //apply post processing to the final fbo draw
    //m_post_outline_shader.use();
    //m_post_outline_shader.set2f(this->getDims(), "dims");
    //m_post_outline_shader.set3f(glm::vec3(-1, -1, -1), "edge_color");
    
    this->drawEnd3D(m_player->getCam());

    //m_post_outline_shader.unuse();
}

void GameLogic::click()
{
    static Engine3D::Game::VSync vsync = Engine3D::Game::VSync::On;
    if (vsync == Engine3D::Game::VSync::On)
    {
        vsync = Engine3D::Game::VSync::Off;
    }
    else
    {
        vsync = Engine3D::Game::VSync::On;
    }
    this->setVSync(vsync);
}

void GameLogic::spawn_explosion(const glm::vec3& pos)
{
    for (u32 i = 0; i < 30; i++)
    {
        m_objects_to_insert.push_back(new Particle(pos, glm::vec3(
            Engine3D::Random::uniform(-1, 1),
            Engine3D::Random::uniform(-1, 1),
            Engine3D::Random::uniform(-1, 1)
        )));
    }
}
