#include "Game.hpp"

#include "Macros.hpp"

#include <chrono>
#include <thread>


#include <SDL2/SDL_image.h>

#include <glm/gtc/type_ptr.hpp>

#include "Sound.hpp"

namespace Engine3D
{
	/**
	 * explicit constructor
	 */
	Game::Game(int argc, const char* argv[], const char* name, u32 width, u32 height, float fov/* = 60*/, float max_fps/* = 60*/)
	{
		//argument correction
		if(max_fps <= 0) { max_fps = 60;   }
		if(fov <= 0)     { fov     = 60;   }
		if(width == 0)   { width   = 1280; }
		if(height == 0)  { height  = 720;  }
		if(argc < 1)           { throw std::invalid_argument("Game::Game() error: please pass valid argc argument"); }
		if(argv[0] == nullptr) { throw std::invalid_argument("Game::Game() error: please pass valid argv argument"); }
	
		//get executable folder path
		m_exe_folder = std::string(argv[0]);

		m_exe_folder = m_exe_folder.substr(0, m_exe_folder.find_last_of("/\\"));

		System::init(argc, argv);
	
		//init SDL
		SDL_Init(SDL_INIT_EVERYTHING);
	
		//create SDL window with OpenGL rendering context
		m_window  = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

		if(m_window == nullptr)
		{
			std::printf("error(): %s\n", SDL_GetError());
		}

		m_context = SDL_GL_CreateContext(m_window);

		//manage vsync
		SDL_GL_SetSwapInterval(0);
	
		//save resolution of the window
		m_dims    = glm::vec2(width, height);
	
		//initialize SDL subsystems
		IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
		TTF_Init();
		Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
		Mix_AllocateChannels(Sound::NumAudioChannels);
	
		//initialize OpenGL
		float screen_ratio = static_cast<float>(width) / static_cast<float>(height);
	
		//setup background color
		glClearColor(0.0, 0.0, 0.0, 1.0);
	
		//setup projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(fov, screen_ratio, Game::FrustrumMin, Game::FrustrumMax);
		
		glMatrixMode(GL_MODELVIEW);
	
		//setup OpenGL variables
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		printf("OpenGL Verison: %s\n", glGetString(GL_VERSION));

		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			std::printf("glewInit() error: %s\n", glewGetErrorString(err));
		}

		//init main fbo
		m_main_fbo.init(width, height);
	
		//limit fps
		m_fps_limiter.setMaxFPS(max_fps);
	}
	
	/**
	 * launch application
	 */
	void Game::run()
	{
		//call user defined initialization
		init();
		
		//move mouse to the center of the window
		SDL_GetMouseState(&m_mouse_pos.x, &m_mouse_pos.y);
		m_mouse_delta = glm::vec2(0);
		SDL_WarpMouseInWindow(m_window, m_dims.x / 2, m_dims.y / 2);
	
		//event buffer
		SDL_Event event;

		//calculate time difference between frames
		float time_delta = 1.0;
	
		//start main loop
		while(m_running)
		{
			//start limiting FPS
			m_fps_limiter.begin();
		
			while(SDL_PollEvent(&event))
			{
				//process event
				switch(event.type)
				{
					//quit the application
					case SDL_QUIT:
					{
						m_running = false; break;
					}
				
					//manage mouse click
					case SDL_MOUSEBUTTONDOWN:
					{
						click();
						break;
					}
				
					//manage mouse click
					case SDL_MOUSEBUTTONUP:
					{
						break;
					}
				
					//manage mouse movement
					case SDL_MOUSEMOTION:
					{
						if(m_mouse_anchored)
						{
							m_mouse_delta = glm::vec2(event.motion.xrel, event.motion.yrel);
						}
						else
						{
							m_mouse_old_pos = m_mouse_pos;
							SDL_GetMouseState(&m_mouse_pos.x, &m_mouse_pos.y);
							m_mouse_delta = m_mouse_pos - m_mouse_old_pos;
						}

						break;
					}
				
					//manage gamepad connection
					case SDL_CONTROLLERDEVICEADDED:
					{
						if(event.cdevice.which == 0) { m_gamepad.use(0); } break;
					}
				
					//manage gamepad connection
					case SDL_CONTROLLERDEVICEREMOVED:
					{
						if(event.cdevice.which == 0) { m_gamepad.unuse(); } break;
					}
				
					//manage gamepad controls
					case SDL_JOYAXISMOTION:
					{
						if(m_gamepad.active()) 
						{
							m_gamepad.m_axes[event.jaxis.axis] = event.jaxis.value;
						}
						break;
					}
				
					//manage gamepad controls
					case SDL_JOYBUTTONDOWN:
					case SDL_JOYBUTTONUP:
					{
						if(m_gamepad.active())
						{
							m_gamepad.m_buttons[event.jbutton.button] = event.jbutton.state;
						}
						break;
					}
				
					default:
					{
						break;
					}
				}
			}
		
			//fetch key states
			m_keys = SDL_GetKeyboardState(nullptr);

			if (keyDown(Key::ESC)) { m_running = false; }
		
			//update game
			update(time_delta);
			
			//swap backbuffer
			SDL_GL_SwapWindow(m_window);
		
			//limit FPS
			m_fps = m_fps_limiter.end();

			time_delta = m_fps_limiter.getMaxFPS() / m_fps;
		}
	
		//user cleanup
		clean();
	
		//cleanup
		m_gamepad.unuse();
		SDL_GL_DeleteContext(m_context);
		SDL_DestroyWindow(m_window);
	}

	/**
	 * drawing management
	 */
	void Game::drawBegin3D(Camera& cam)
	{
		

		//clear screen
		glClearColor(m_background_color.x, m_background_color.y, m_background_color.z, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		//update projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(cam.getFov(), static_cast<float>(m_dims.x) / static_cast<float>(m_dims.y), FrustrumMin, FrustrumMax);

		//update view matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();



		//update cam view
		glMultMatrixf(value_ptr(cam.getViewMatrix()));

		//draw to main fbo
		m_main_fbo.bind();
	}

	/**
	 * drawing management
	 */
	void Game::drawEnd3D(Camera& cam)
	{
		m_main_fbo.unbind();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		m_main_fbo.draw();
	}

	void Game::disableDepthTest()
	{
		glDepthFunc(GL_ALWAYS);
	}
	void Game::enableDepthTest()
	{
		glDepthFunc(GL_LESS);
	}
}