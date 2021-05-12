/*
This file is part of Engine3D.

Engine3D is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Engine3D is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Engine3D.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "Game.hpp"

#include "Macros.hpp"

#include <chrono>
#include <thread>
#include <algorithm>

#include <SDL2/SDL_image.h>

#include <glm/gtc/type_ptr.hpp>

#include "Sound.hpp"
#include "TimeInterval.hpp"

namespace Engine3D
{
    /**
     * explicit constructor
     */
    Game::Game(int argc, const char** argv, const char* name, u32 width, u32 height, float max_fps)
    {
        init(argc, argv, name, width, height, max_fps);
    }

    void Game::init(int argc, const char** argv, const char* name, u32 width, u32 height, float max_fps/* = 60*/)
    {
        std::printf("[Game]: initializing components...\n");
        m_timer.start();

        //argument correction
        if (max_fps <= 0) { max_fps = 60; }
        if (width   == 0) { width = 1280; }
        if (height  == 0) { height = 720; }
        if (argc     < 1)       { throw std::invalid_argument("Game::Game() error: please pass valid argc argument"); }
        if (argv[0] == nullptr) { throw std::invalid_argument("Game::Game() error: please pass valid argv argument"); }

        m_argc = argc;
        m_argv = argv;

        //get executable folder path
        m_exe_folder = std::string(argv[0]);

        m_exe_folder = m_exe_folder.substr(0, m_exe_folder.find_last_of("/\\"));

        System::init(argc, argv);

        //init SDL
        SDL_Init(SDL_INIT_EVERYTHING);

        //create SDL window with OpenGL rendering context
        m_window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

        if (m_window == nullptr)
        {
            std::printf("[Game] error(): %s\n", SDL_GetError());  return;
        }

        m_context = SDL_GL_CreateContext(m_window);

        //manage vsync
        SDL_GL_SetSwapInterval(0);

        //save resolution of the window
        m_dims = glm::vec2(width, height);

        //initialize SDL subsystems
        IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
        TTF_Init();
        Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
        Mix_AllocateChannels(Sound::NumAudioChannels);

        //initialize OpenGL

        //setup projection
        m_proj_width_over_height = static_cast<float>(width) / static_cast<float>(height);
        m_proj_near  = Game::DefaultFrustrumMin;
        m_proj_far   = Game::DefaultFrustrumMax;
        m_proj_left  = -(width / 2.0f);
        m_proj_right =  width / 2.0f;
        m_proj_down  = -(height / 2.0f);
        m_proj_up    =  height / 2.0f;

        //setup background color
        glClearColor(0.0, 0.0, 0.0, 1.0);

        //setup projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glMatrixMode(GL_MODELVIEW);

        //setup OpenGL variables
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        printf("[Game] OpenGL Version: %s\n", glGetString(GL_VERSION));

        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
            std::printf("[Game] glewInit() error: %s\n", glewGetErrorString(err));
        }

        //init main fbo
        m_main_fbo.init(width, height);

        //limit fps
        m_fps_limiter.setMaxFPS(max_fps);

        std::printf("[Game] initializing components finished in: %f s\n", m_timer.end());
    }

    Game::~Game()
    {
        destroy();
    }

    void Game::destroy()
    {
        m_main_fbo.clean();
        SDL_DestroyWindow(m_window);
        SDL_GL_DeleteContext(m_context);
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    /**
     * launch application
     */
    void Game::run()
    {
        //move mouse to the center of the window
        /*
        SDL_GetMouseState(&m_mouse_pos.x, &m_mouse_pos.y);
        m_mouse_delta = glm::vec2(0);
        SDL_WarpMouseInWindow(m_window, m_dims.x / 2, m_dims.y / 2);
        */

        std::printf("[Game] initializing keyboard, screen and timing...\n");
        m_timer.start();
        int keyboard_len = 0;
        m_keys = SDL_GetKeyboardState(&keyboard_len);

        //event buffer
        SDL_Event event;

        //preswap the window a few times -> reduces initial lag
        SDL_GL_SwapWindow(m_window);
        SDL_GL_SwapWindow(m_window);
        SDL_GL_SwapWindow(m_window);
        SDL_GL_SwapWindow(m_window);

        //start capturing frames
        float previous_time = static_cast<float>(SDL_GetTicks());

        m_fps_limiter.enabled() = false;

        u32 fps_counter = 0;

        std::printf("[Game] initializing keyboard, screen and timing finished in %f s\n", m_timer.end());

        std::printf("[Game] initializing game...\n");
        m_timer.start();
        //call user defined initialization
        init();

        std::printf("[Game] initializing game finished in: %f s\n", m_timer.end());

        std::printf("[Game] started main loop\n");

        //start main loop
        while (m_running)
        {
            m_fps_limiter.begin();

            while (SDL_PollEvent(&event))
            {
                //process event
                switch (event.type)
                {
                    //quit the application
                case SDL_QUIT: { m_running = false; break; }

                             //manage mouse click
                case SDL_MOUSEBUTTONDOWN: { click(); break; }

                                        //manage mouse click
                case SDL_MOUSEBUTTONUP: { break; }

                                      //manage mouse movement
                case SDL_MOUSEMOTION:
                {
                    if (m_mouse_anchored)
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
                    if (event.cdevice.which == 0) { m_gamepad.use(0); } break;
                }

                //manage gamepad connection
                case SDL_CONTROLLERDEVICEREMOVED:
                {
                    if (event.cdevice.which == 0) { m_gamepad.unuse(); } break;
                }

                //manage gamepad controls
                case SDL_JOYAXISMOTION:
                {
                    if (m_gamepad.active())
                    {
                        m_gamepad.m_axes[event.jaxis.axis] = event.jaxis.value;
                    }
                    break;
                }

                //manage gamepad controls
                case SDL_JOYBUTTONDOWN:
                case SDL_JOYBUTTONUP:
                {
                    if (m_gamepad.active())
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

            //capture start of the frame
            float new_time         = static_cast<float>(SDL_GetTicks());
            float frame_time       = new_time - previous_time;
            previous_time          = new_time;
            float total_delta_time = frame_time / (Game::MSPerSecond / m_fps_limiter.getMaxFPS());
            
            
            //update game
            int i = 0;
            while (total_delta_time > 0 && i < Game::MaxPhysicalSteps)
            {
                float delta_time = std::min(total_delta_time, Game::MaxDeltaTime);
                update(delta_time * m_game_speed);
                total_delta_time -= delta_time;
                i++;
            }

            draw();

            //swap backbuffer
            SDL_GL_SwapWindow(m_window);
            
            m_fps = m_fps_limiter.end();
            //std::printf("[Game] fps: %f\n", m_fps);
        }

        //user cleanup
        clean();

        //cleanup
        m_gamepad.unuse();
        SDL_GL_DeleteContext(m_context);
        SDL_DestroyWindow(m_window);
    }

    void Game::doPerspectiveProjection(float fov, float width_over_height, float near_plane, float far_plane, Engine3D::Camera& cam)
    {
        cam.setFov(fov);
        m_proj_width_over_height = width_over_height;
        m_proj_near              = near_plane;
        m_proj_far               = far_plane;

        m_projection = ProjectionType::Perspective;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(cam.getFov(), m_proj_width_over_height, m_proj_near, m_proj_far);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //update cam view
        glMultMatrixf(glm::value_ptr(cam.getViewMatrix()));
    }
    void Game::doOrthographicProjection(float left, float right, float down, float up, float near_plane, float far_plane, Engine3D::Camera& cam)
    {
        m_proj_left  = left;
        m_proj_right = right;
        m_proj_down  = down;
        m_proj_up    = up;
        m_proj_near  = near_plane;
        m_proj_far   = far_plane;

        m_projection = ProjectionType::Orthographic;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(m_proj_left, m_proj_right, m_proj_down, m_proj_up, m_proj_near, m_proj_far);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //update cam view
        glMultMatrixf(glm::value_ptr(cam.getViewMatrix()));
    }

    /**
     * drawing management
     */
    void Game::drawBegin3D(Engine3D::Camera& cam, bool clear)
    {
        //clear screen
        if (clear)
        {
            glClearColor(m_background_color.x, m_background_color.y, m_background_color.z, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        //update projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (m_projection == ProjectionType::Perspective)
        {
            gluPerspective(cam.getFov(), m_proj_width_over_height, m_proj_near, m_proj_far);
        }
        else
        {
            glOrtho(m_proj_left, m_proj_right, m_proj_down, m_proj_up, m_proj_near, m_proj_far);
        }

        //update view matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        //update cam view
        glMultMatrixf(glm::value_ptr(cam.getViewMatrix()));

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

    void Game::setFullScreenMode()
    {
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
    }

    void Game::setWindowedMode()
    {
        SDL_SetWindowFullscreen(m_window, 0);
    }

    void Game::setFullScreenWindowedMode()
    {
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    void Game::changeWindowSize(u32 width, u32 height)
    {
        SDL_SetWindowSize(m_window, width, height);
        m_dims.x = width; m_dims.y = height;
        m_main_fbo.init(width, height);
        glViewport(0, 0, width, height);
    }
}
