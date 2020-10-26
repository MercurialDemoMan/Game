#include "GameLogic.hpp"

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

	m_player.reset();
	this->setMouseAnchor(this->getDims() / 2.f);

	//initialize spatial partition for static triangles
	m_spatial_partition.init(Engine3D::SpatialPartition<Engine3D::Triangle>::DefaultCellSize, [this](const Engine3D::Triangle& triangle) -> Engine3D::BoundingBox
		{
			Engine3D::BoundingBox r;
			
			r.min.x = std::min(triangle.p1.x, std::min(triangle.p2.x, triangle.p3.x)) - m_player.dims().y;
			r.min.y = std::min(triangle.p1.y, std::min(triangle.p2.y, triangle.p3.y)) - m_player.dims().y;
			r.min.z = std::min(triangle.p1.z, std::min(triangle.p2.z, triangle.p3.z)) - m_player.dims().y;

			r.max.x = std::max(triangle.p1.x, std::max(triangle.p2.x, triangle.p3.x)) + m_player.dims().y;
			r.max.y = std::max(triangle.p1.y, std::max(triangle.p2.y, triangle.p3.y)) + m_player.dims().y;
			r.max.z = std::max(triangle.p1.z, std::max(triangle.p2.z, triangle.p3.z)) + m_player.dims().y;

			return r;
		});

	//load shaders
	Engine3D::inline_try<std::runtime_error>([&]
		{
			m_obj_shader.init("data/shaders/objects.vert", "data/shaders/objects.frag");
			m_image_shader.init("data/shaders/image.vert", "data/shaders/image.frag");
			m_skybox_shader.init("data/shaders/skybox.vert", "data/shaders/skybox_shapes.frag");
			m_post_outline_shader.init("data/shaders/scene_post.vert", "data/shaders/scene_post_outline.frag");
		}, "GameLogic::Engine3D_init()");
	
	//load lights
	Engine3D::inline_try<std::runtime_error>([&]
		{
			m_light = new Light(glm::vec3(0), "data/textures/light.png");
			m_light->scale() *= 0.01;
			//m_light->diffuse_color() = m_light->specular_color() = glm::vec3(0.8f, 0.2f, 0.2f);
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

	//load level
	Engine3D::inline_try<std::runtime_error>([&]
		{
			//TODO: load level in correct json format
			Engine3D::JSONDocument json("data/levels/test.json");

			Engine3D::JSONObject& o = json["objects"];

			o.for_each_object([&](Engine3D::JSONObject& world_object)
			{
				glm::vec3    object_pos   = glm::vec3(world_object["x"].number_data(), 
					                                  world_object["y"].number_data(), 
					                                  world_object["z"].number_data());
				float        object_dim   = world_object["s"].number_data();
				std::string& object_model = world_object["m"].string_data();
				Object::HitboxType object_type = static_cast<Object::HitboxType>((u32)world_object["t"].number_data());

				m_objects.push_back(new Object(object_pos, object_model));
				m_objects.back()->scale()  = m_objects.back()->dims() = glm::vec3(object_dim);
				m_objects.back()->hitbox() = object_type;
				m_objects.back()->setupVertexAttributes(m_obj_shader);
			});

		}, "GameLogic::Engine3D_init()");

	//load skybox
	Engine3D::inline_try<std::runtime_error>([&]
		{
			m_skybox.init(glm::vec3(0), "data/objects/inv_cube.obj");
			m_skybox.scale() = glm::vec3(Engine3D::Game::FrustrumMax / sqrtf(3));
			m_skybox.setupVertexAttributes(m_skybox_shader);
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

	std::printf("GameLogic() log: constructing spatial partitions...\n");

	//construct spatial partition
	for (auto& object : m_objects)
	{
		if (object->hitbox() != Object::HitboxType::Mesh)
		{
			continue;
		}

		const std::vector<Engine3D::Vertex>* vertices = object->vertices();

		if (vertices->size() != 0)
		{
			for (u32 i = 0; i < vertices->size(); i += 3)
			{
				m_triangles.push_back(object->constructTriangle(i));
			}
		}
	}

	m_spatial_partition.update(m_triangles);

	for (auto& object : m_objects)
	{
		object->discardVertices();
	}

	std::printf("GameLogic() log: spatial partitions constructed\n");
}

void GameLogic::clean()
{
	this->freeMouse();

	for (auto& object : m_objects)
	{
		delete object;
	}

	delete m_light;

	m_main_music.stop();
}

void GameLogic::update(const float time_delta)
{
	//TODO: somethims we just fall through the floor
	//TODO: time delta movement is fucked

	if(m_pause == false)
	{
		

		//TODO: update time
		m_time += (float)fmod(1.0f * time_delta, M_PI * 2.0);

		//save vertical movement for playing the right land sound
		float player_prev_mov_y = m_player.mov().y;
		bool  player_prev_on_ground = m_player.on_ground();

		//apply gravity to the player
		m_player.mov().y -= Player::Gravity;
		m_player.mov().y  = std::clamp(m_player.mov().y, -Player::MaxVerticalSpeed, Player::MaxVerticalSpeed);

		m_player.addPos(glm::vec3(0, m_player.mov().y * time_delta * time_delta, 0));

		//reset on ground flag and update coyote jump
		if (m_player.coyote_jump() <= 0.0f)
		{
			m_player.on_ground()       = false;
			m_player.on_ground_slope() = glm::vec3(0, 1, 0);
		}
		else
		{
			m_player.coyote_jump() -= 1.0f * time_delta;
		}

		//normalize player's forward direction
		glm::vec2 player_dir = glm::normalize(glm::vec2(m_player.getDir().x, m_player.getDir().z));

		//flags for to know when to slow down
		bool player_move_straight = false;
		bool player_move_sideways = false;

		//move forward
		if (this->keyDown(Engine3D::Game::Key::W) && !this->keyDown(Engine3D::Game::Key::S))
		{
			m_player.mov().z += player_dir.y * Player::Acceleration * time_delta;
			m_player.mov().x += player_dir.x * Player::Acceleration * time_delta;
			player_move_straight = true;
		}

		//move backward
		if (this->keyDown(Engine3D::Game::Key::S) && !this->keyDown(Engine3D::Game::Key::W))
		{
			m_player.mov().z -= player_dir.y * Player::Acceleration * time_delta;
			m_player.mov().x -= player_dir.x * Player::Acceleration * time_delta;
			player_move_straight = true;
		}

		//get sideways vector
		player_dir = glm::rotate(player_dir, static_cast<float>(M_PI_2));

		//move right
		if (this->keyDown(Engine3D::Game::Key::D) && !this->keyDown(Engine3D::Game::Key::A))
		{
			m_player.mov().z -= player_dir.y * Player::Acceleration * time_delta;
			m_player.mov().x -= player_dir.x * Player::Acceleration * time_delta;
			player_move_sideways = true;
		}

		//move left
		if (this->keyDown(Engine3D::Game::Key::A) && !this->keyDown(Engine3D::Game::Key::D))
		{
			m_player.mov().z += player_dir.y * Player::Acceleration * time_delta;
			m_player.mov().x += player_dir.x * Player::Acceleration * time_delta;
			player_move_sideways = true;
		}
		
		//collision
		//reset plane
		/*if (m_player.getPos().y - m_player.dims().y / 2 <= -200.0f)
		{
			m_player.reset();
		}*/

		m_objects.back()->pos().x += cos(m_time / 50.0f) / 5.0f;
		m_objects.back()->pos().z += cos(m_time / 50.0f) / 5.0f;
		m_objects.back()->pos().y += cos(m_time / 100.0f) / 5.0f;

		glm::vec3 mov = glm::vec3(cos(m_time / 50.0f) / 5.0f, cos(m_time / 50.0f) / 5.0f, cos(m_time / 100.0f) / 5.0f);

		if(Engine3D::Collision::BoxVsBoxSweep(m_objects[m_objects.size() - 1]->pos(), m_objects[m_objects.size() - 1]->dims(), mov, m_objects[m_objects.size() - 2]->pos(), m_objects[m_objects.size() - 2]->dims()))
		{
			const_cast<Engine3D::Material*>(m_objects.back()->material())->diffuse = glm::vec3(1, 0, 0);
			const_cast<Engine3D::Material*>(m_objects.back()->material())->ambient = glm::vec3(0.2, 0, 0);
			const_cast<Engine3D::Material*>(m_objects.back()->material())->specular = glm::vec3(1, 0, 0);
		}
		else
		{
			const_cast<Engine3D::Material*>(m_objects.back()->material())->diffuse = glm::vec3(1, 1, 1);
			const_cast<Engine3D::Material*>(m_objects.back()->material())->ambient = glm::vec3(0.2, 0.2, 0.2);
			const_cast<Engine3D::Material*>(m_objects.back()->material())->specular = glm::vec3(1, 1, 1);
		}	

		glm::vec3 collision_result(0);

		//perform collision with static objects
		auto triangles = m_spatial_partition.get(m_player.getPos());

		if (triangles != nullptr)
		{
			
			for (auto& triangle : *triangles)
			{
				glm::vec3 collision = Engine3D::Collision::EllipsoidVsTriangle(m_player.getPos(), m_player.dims(), triangle->p1, triangle->p2, triangle->p3).displacement;
				m_player.addPos(collision);
				collision_result += collision;
				//if(collision_result != glm::vec3(0)) { break; }
			}
		}

		for (auto& object : m_objects)
		{
			glm::vec3 collision(0);

			switch (object->hitbox())
			{
				//Box vs Box
				case Object::HitboxType::Box:
				{
					//Engine3D::Collision::Data collision_data = Engine3D::Collision::BoxVsBox(m_player.getPos(), m_player.dims(), object->pos(), object->dims());
					//Engine3D::Collision::Data collision_data = Engine3D::Collision::BoxVsBoxSweep(m_player.getPos(), m_player.dims(), m_player.mov(), object->pos(), object->dims());
					
					Engine3D::Collision::Data collision_data = Engine3D::Collision::BoxVsBox(m_player.getPos(), m_player.dims(), object->pos(), object->dims());
					if (collision_data.occurred)
					{
						//std::printf("%f, %f, %f\n", collision_data.displacement.x, collision_data.displacement.y, collision_data.displacement.z);
						collision = collision_data.displacement;
					}
					break;
				}
				//Cylinder vs Cylinder
				case Object::HitboxType::Cylinder:
				{
					Engine3D::Collision::Data collision_data = Engine3D::Collision::CylinderVsCylinder(m_player.getPos(), glm::vec2(m_player.dims().x / 2, m_player.dims().y), object->pos(), glm::vec2(object->dims().x / 2, object->dims().y));
					if (collision_data.occurred)
					{
						collision = collision_data.displacement;
					}
					break;
				}
				//Ball vs Ball
				case Object::HitboxType::Ball:
				{
					Engine3D::Collision::Data collision_data = Engine3D::Collision::BallVsBall(m_player.getPos(), m_player.dims().y / 2, object->pos(), object->dims().y / 2);
					if (collision_data.occurred)
					{
						collision = collision_data.displacement;
					}
					break;
				}
				//Ball vs Triangles
				case Object::HitboxType::Mesh:
				case Object::HitboxType::None:
				default:
				{
					break;
				}
			}

			if (collision != glm::vec3(0))
			{
				if (object->collision_trigger())
				{
					std::invoke(object->collision_trigger());
				}

				if (object->has_collision_response())
				{
					m_player.addPos(collision);
					collision_result += collision;
				}
			}
		}
		
		//if there was a collision, adjust player flags and player movement
		if (collision_result != glm::vec3(0))
		{
			glm::vec3 normalized_res = glm::normalize(collision_result);

			//check if the slope is flat enough
			if (collision_result.y > 0.0f)
			{
				if(glm::angle(normalized_res, glm::vec3(0, 1, 0)) < ENGINE3D_DEGTORAD(45.0f / m_player.dims().y))
				{
					//set "jump" and "on ground" flags
					m_player.mov().y           = 0.0f;
					m_player.on_ground()       = true;
					m_player.on_ground_slope() = normalized_res;
					m_player.can_jump()        = true;
					m_player.coyote_jump()     = Player::CoyoteJumpDefault;

					if (normalized_res != glm::vec3(0, 1, 0))
					{
						//manipulate collision result, so we are not sliding down from almost flat slopes
						float angle = glm::angle(collision_result, glm::vec3(0, 1, 0));

						if (angle != 0.0f)
						{
							float cotangent = 1.0f / tanf(angle);
							cotangent = cotangent * glm::length(collision_result);
							m_player.addPos(glm::vec3(0, cotangent, 0) - collision_result);
						}
					}
				}
			}

			auto is_valid = [](glm::vec3& vector)
			{
				return !(isnan(vector.x) || isnan(vector.y) || isnan(vector.z) || isinf(vector.x) || isinf(vector.y) || isinf(vector.z));
			};

			//modify players movement
			glm::vec3 new_dir = glm::cross(collision_result, m_player.mov());
			new_dir = glm::cross(new_dir, collision_result);
			new_dir = -glm::normalize(new_dir);
			if (is_valid(new_dir))
			{
				m_player.mov() = new_dir * glm::dot(new_dir, m_player.mov());
				m_player.addPos(m_player.mov() * time_delta);
			}
			else
			{
				m_player.mov() = glm::vec3(0);
			}
		}
		else
		{
			//apply movement force
			m_player.addPos(glm::vec3(m_player.mov().x * time_delta, 0, m_player.mov().z * time_delta));
		}		

		//add landing bob
		if (player_prev_on_ground == false && m_player.on_ground() == true &&
			player_prev_mov_y < Player::LandBobThreshold)
		{
			m_player.land_bob() = Player::LandBob;
			m_land_sound.play();
		}

		//jump
		static bool jump_down = false;
		if(this->keyDown(Engine3D::Game::Key::SPACE) && !jump_down)
		{
			if (m_player.can_jump())
			{
				m_player.coyote_jump() = 0;
				m_player.can_jump()    = false;

				if (m_player.on_ground())
				{
					m_player.mov() += m_player.on_ground_slope() * Player::JumpPower / time_delta;
				}

				m_jump_sound.play();
			}
		}

		//slowing down the player
		if (m_player.on_ground())
		{
			if (!player_move_sideways && !player_move_straight)
			{
				m_player.mov().z -= m_player.mov().z * Player::Decceleration * time_delta; if(fabs(m_player.mov().z) < 0.001f) { m_player.mov().z = 0; }
				m_player.mov().x -= m_player.mov().x * Player::Decceleration * time_delta; if(fabs(m_player.mov().x) < 0.001f) { m_player.mov().x = 0; }
			}
		}

		//clamping horizontal speed
		if (std::sqrt(m_player.mov().x * m_player.mov().x + m_player.mov().z * m_player.mov().z) > Player::MaxHorizontalSpeed)
		{
			player_dir = glm::normalize(glm::vec2(m_player.mov().x, m_player.mov().z));
			m_player.mov().x = player_dir.x * Player::MaxHorizontalSpeed;
			m_player.mov().z = player_dir.y * Player::MaxHorizontalSpeed;
		}

		glm::vec2 mouse_delta = this->getMouseDelta() * time_delta;

		//rotate camera around y axis
		m_player.getCam().rotate(glm::vec3(0, 1, 0), mouse_delta.x / 100.0f);

		//rotate camera around its right vector + gimbal lock the rotation to only 180 deg
		//TODO: if window loses focus and regains it back, it can lock the camera in the 90 or -90 degrees
		glm::vec3 cam_dir_right = glm::cross(glm::vec3(0, 1, 0), m_player.getCam().getDir());
		glm::vec3 old_cam_dir   = glm::normalize(glm::vec3(m_player.getCam().getDir().x, 0, m_player.getCam().getDir().z));
		glm::vec3 new_cam_dir   = glm::rotate(m_player.getCam().getDir(), mouse_delta.y / 100.0f, cam_dir_right);

		float angle_delta = glm::angle(old_cam_dir, new_cam_dir) * 180.0f / static_cast<float>(M_PI);

		if (m_player.getCam().getDir().y != 0)
		{
			angle_delta *= -m_player.getCam().getDir().y / fabs(m_player.getCam().getDir().y);

			if (angle_delta >= 89.0f)
			{
				m_player.getCam().setDir(glm::rotate(old_cam_dir, static_cast<float>(89.0f * M_PI / 180.0f), cam_dir_right));
			}
			else if (angle_delta <= -89.0f)
			{
				m_player.getCam().setDir(glm::rotate(old_cam_dir, static_cast<float>(-89.0f * M_PI / 180.0f), cam_dir_right));
			}
			else
			{
				m_player.getCam().rotate(cam_dir_right, mouse_delta.y / 100.0f);
			}
		}
		else
		{
			m_player.getCam().rotate(cam_dir_right, mouse_delta.y / 100.0f);
		}
		
		//apply bobbing effect when running
		m_player.bobbing() = fabs(sinf(m_time * 0.01f * 20.0f)) * sqrtf(powf(m_player.mov().z, 2.0f) + powf(m_player.mov().x, 2.0f)) * 1.5f * m_player.on_ground();

		//smooth out landing bob
		m_player.land_bob() -= m_player.land_bob() / 20.0f * time_delta;

		//play step sound
		if (fabs(m_player.mov().x) > 0.05f || fabs(m_player.mov().z) > 0.05f)
		{
			if (m_player.on_ground())
			{
				if ((sinf(m_time * 0.01f * 20.0f) <= 0.0f && sinf((m_time - 1) * 0.01f * 20.0f) >= 0.0f) ||
					(sinf(m_time * 0.01f * 20.0f) >= 0.0f && sinf((m_time - 1) * 0.01f * 20.0f) <= 0.0f))
				{
					if (m_player.step())
					{
						m_step_left_sound.play();
					}
					else
					{
						m_step_right_sound.play();
					}
					m_player.step() = !m_player.step();
				}
			}
		}

		//set fov based on how fast are we moving
		m_player.getCam().setFov(m_player.getCam().getFov() + (100.0f + (sqrt(pow(m_player.mov().x, 2.0f) + pow(m_player.mov().z, 2.0f)) * 50.0f) - m_player.getCam().getFov()) / 10.0f);
	
		//update skybox position
		m_skybox.pos() = m_player.getPos();
	}

	m_light->pos() = glm::vec3(sinf(m_time / 100.0f) * 10.0f, -20.0f + sinf(m_time / 200.0f) * 10.0f, cosf(m_time / 100.0f) * 10.0f);

	

	this->drawBegin3D(m_player.getCam());

	
	//TODO: remove drawing hitboxes
	auto draw_hitbox = [](const glm::vec3& pos, const glm::vec3& dims)
	{
		glBegin(GL_LINES);
		glVertex3f(pos.x + dims.x / 2, pos.y + dims.y / 2, pos.z + dims.z / 2);
		glVertex3f(pos.x + dims.x / 2, pos.y - dims.y / 2, pos.z + dims.z / 2);
		glVertex3f(pos.x + dims.x / 2, pos.y + dims.y / 2, pos.z + dims.z / 2);
		glVertex3f(pos.x - dims.x / 2, pos.y + dims.y / 2, pos.z + dims.z / 2);
		glVertex3f(pos.x + dims.x / 2, pos.y + dims.y / 2, pos.z + dims.z / 2);
		glVertex3f(pos.x + dims.x / 2, pos.y + dims.y / 2, pos.z - dims.z / 2);

		glVertex3f(pos.x - dims.x / 2, pos.y + dims.y / 2, pos.z - dims.z / 2);
		glVertex3f(pos.x - dims.x / 2, pos.y - dims.y / 2, pos.z - dims.z / 2);
		glVertex3f(pos.x + dims.x / 2, pos.y - dims.y / 2, pos.z - dims.z / 2);
		glVertex3f(pos.x - dims.x / 2, pos.y - dims.y / 2, pos.z - dims.z / 2);
		glVertex3f(pos.x - dims.x / 2, pos.y - dims.y / 2, pos.z + dims.z / 2);
		glVertex3f(pos.x - dims.x / 2, pos.y - dims.y / 2, pos.z - dims.z / 2);
		glEnd();
	};
	auto draw_hitboxes = [&]()
	{
		glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(-1, 0, 0);
		glVertex3f(1, 0, 0);
		glColor3f(0, 1, 0);
		glVertex3f(0, -1, 0);
		glVertex3f(0, 1, 0);
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, -1);
		glVertex3f(0, 0, 1);
		glColor3f(1, 1, 1);
		glEnd();
		
		draw_hitbox(m_player.getPos(), m_player.dims());
		for (auto& object : m_objects)
		{
			draw_hitbox(object->pos(), object->dims());
		}

		auto triangles = m_spatial_partition.get(m_player.getPos());

		if (triangles != nullptr)
		{
			for (auto& triangle : *triangles)
			{
				glBegin(GL_LINES);
				glVertex3f(triangle->p1.x, triangle->p1.y, triangle->p1.z);
				glVertex3f(triangle->p2.x, triangle->p2.y, triangle->p2.z);
				glVertex3f(triangle->p1.x, triangle->p1.y, triangle->p1.z);
				glVertex3f(triangle->p3.x, triangle->p3.y, triangle->p3.z);
				glVertex3f(triangle->p3.x, triangle->p3.y, triangle->p3.z);
				glVertex3f(triangle->p2.x, triangle->p2.y, triangle->p2.z);
				glEnd();
			}
		}

		Engine3D::BoundingBox cell_rect = m_spatial_partition.getCell(m_player.getPos());
		glColor3f(1, 0, 0);
		draw_hitbox(cell_rect.min + (cell_rect.max - cell_rect.min) / 2.0f, glm::vec3((float)m_spatial_partition.getCellSize()));
		glColor3f(1, 1, 1);
	};
	
	draw_hitboxes();

	//draw skybox
	m_skybox_shader.use();

	m_skybox_shader.setTextureCubemap(m_skybox_texture.getID(),    "skybox_texture");
	m_skybox_shader.setTexture2D(m_skybox_noise.getID(),           "noise_texture");
	m_skybox_shader.set4x4m(m_player.getCam().getRotationMatrix(), "rotation_matrix");
	m_skybox_shader.set3f(glm::vec3(0.5, 0.5, 1),                  "color");
	m_skybox_shader.set1f(m_time / 100.0f,                         "time");

	m_skybox.draw();

	m_skybox_shader.unuse();

	//draw static object

	for(auto& object : m_objects)
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

			m_obj_shader.set3f(object->material()->ambient, "material_ambient");
			m_obj_shader.set3f(object->material()->diffuse, "material_diffuse");
			m_obj_shader.set3f(object->material()->specular, "material_specular");
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
		m_obj_shader.set3f(/*m_player.getPos()*/m_light->pos(), "light_pos");
		m_obj_shader.set4x4m(m_player.getCam().getRotationMatrix(), "rot_mat");
		m_obj_shader.set4x4m(m_player.getCam().getViewMatrix(), "view_mat");
		m_obj_shader.set1f(m_time / 100.0f, "time");
		m_obj_shader.set3f(object->pos(), "obj_pos");

		object->draw();

		m_obj_shader.unuse();
	}

	m_image_shader.use();
	m_image_shader.setTexture2D(m_light->texture().getID(), "image");
	m_image_shader.set3f(m_light->diffuse_color(), "color");

	m_light->draw(m_player.getCam());

	m_image_shader.unuse();
	
	//apply post processing to the final fbo draw
	m_post_outline_shader.use();
	m_post_outline_shader.set2f(this->getDims(), "dims");
	m_post_outline_shader.set3f(glm::vec3(-1, -1, -1), "edge_color");
	
	this->drawEnd3D(m_player.getCam());
	
	m_post_outline_shader.unuse();
	
	//draw fps
	/*
	m_fps = std::to_string(static_cast<u32>(this->getFPS())) + " FPS";
	m_fps.pos() = glm::vec2(-this->getDims().x + m_fps.getDims().x / 2.0f, this->getDims().y - m_fps.getDims().y / 2.0f);
	m_fps.draw();
	*/
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