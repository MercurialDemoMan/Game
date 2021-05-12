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
#pragma once

#include <functional>
#include <numeric>

#include "Camera.hpp"
#include "FPSLimiter.hpp"
#include "Gamepad.hpp"
#include "Types.hpp"
#include "FBObject.hpp"
#include "TimeInterval.hpp"

namespace Engine3D
{
    /**
     * structure for managing game and drawing into window
     */
    class Game
    {
    public:

        static constexpr const u32   MaxPhysicalSteps = 6;
        static constexpr const float MaxDeltaTime     = 1;
        static constexpr const float MSPerSecond      = 1000;

        /**
         * key enumeration
         */
        enum class Key
        {
            Q = SDL_SCANCODE_Q,
            W = SDL_SCANCODE_W,
            E = SDL_SCANCODE_E,
            R = SDL_SCANCODE_R,
            T = SDL_SCANCODE_T,
            Z = SDL_SCANCODE_Z,
            U = SDL_SCANCODE_U,
            I = SDL_SCANCODE_I,
            O = SDL_SCANCODE_O,
            P = SDL_SCANCODE_P,
            A = SDL_SCANCODE_A,
            S = SDL_SCANCODE_S,
            D = SDL_SCANCODE_D,
            F = SDL_SCANCODE_F,
            G = SDL_SCANCODE_G,
            H = SDL_SCANCODE_H,
            J = SDL_SCANCODE_J,
            K = SDL_SCANCODE_K,
            L = SDL_SCANCODE_L,
            Y = SDL_SCANCODE_Y,
            X = SDL_SCANCODE_X,
            C = SDL_SCANCODE_C,
            V = SDL_SCANCODE_V,
            B = SDL_SCANCODE_B,
            N = SDL_SCANCODE_N,
            M = SDL_SCANCODE_M,
            SPACE  = SDL_SCANCODE_SPACE,
            LSHIFT = SDL_SCANCODE_LSHIFT,
            RHISFT = SDL_SCANCODE_RSHIFT,
            LCTRL  = SDL_SCANCODE_LCTRL,
            RCTRL  = SDL_SCANCODE_RCTRL,
            LEFT   = SDL_SCANCODE_LEFT,
            RIGHT  = SDL_SCANCODE_RIGHT,
            UP     = SDL_SCANCODE_UP,
            DOWN   = SDL_SCANCODE_DOWN,
            ESC    = SDL_SCANCODE_ESCAPE
        };
        
        /**
         * possible vsync values
         */
        enum class VSync
        {
            Off = 0,      // max fps, no monitor synchronization, possible screen tearing
            On  = 1,      // fps synchronized with the monitor, clean image
            Adaptive = -1
        };

        /**
         * explicit constructor
         */
        Game(int argc, const char* argv[], const char* name, u32 width, u32 height, float max_fps = 60);
        ~Game();

        /**
         * user defined functions
         */
        virtual void init() {}
        virtual void clean() {}
        virtual void update(const float time_delta) { ENGINE3D_UNUSED(time_delta); }
        virtual void draw() {}
        virtual void click() {}
        
        /**
         * launch application
         */
        void run();
        
        /**
         * drawing management
         */
        void drawBegin3D(Camera& cam, bool clear = true);
        void drawEnd3D(Camera& cam);
        void setBackgroundColor(float r, float g, float b) { m_background_color = glm::vec3(r, g, b); }
        void setBackgroundColor(const glm::vec3& color)    { m_background_color = color; }
        void disableDepthTest();
        void enableDepthTest();
        void doPerspectiveProjection(float fov, float width_over_height, float near_plane, float far_plane, Engine3D::Camera& cam);
        void doOrthographicProjection(float left, float right, float down, float up, float near_plane, float far_plane, Engine3D::Camera& cam);

        /**
         * manage vsync
         */
        void setVSync(VSync value) { SDL_GL_SetSwapInterval(static_cast<int>(value)); }

        /**
         * set window modes
         */
        void setFullScreenMode();
        void setWindowedMode();
        void setFullScreenWindowedMode();

        /**
         * change window size 
         */
        void changeWindowSize(u32 width, u32 height);

        /**
         * get mouse position
         */
        const glm::vec2& getMousePos() { return m_mouse_pos; }
        
        /**
         * get mouse movement
         */
        glm::vec2 getMouseDelta() { glm::vec2 backup = m_mouse_delta; m_mouse_delta = glm::vec2(0, 0); return backup; }
        
        /**
         * access main fbo
         */
        FBObject& mainFBO() { return m_main_fbo; }

        /**
         * get keyboard state
         */
        bool keyDown(Key key) { return m_keys[static_cast<u8>(SDL_SCANCODE_TO_KEYCODE(static_cast<int>(key)))]; }

        /**
         * managing mouse movement
         */
        void setMousePos(glm::vec2& pos)   { SDL_WarpMouseInWindow(m_window, static_cast<int>(pos.x), static_cast<int>(pos.y)); }
        void setMousePos(glm::vec2&& pos)  { SDL_WarpMouseInWindow(m_window, static_cast<int>(pos.x), static_cast<int>(pos.y)); }
        void setMouseAnchor(glm::vec2 pos) { m_mouse_anchored = true; m_mouse_anchor = pos; setMousePos(m_mouse_anchor); SDL_SetRelativeMouseMode(SDL_TRUE); }
        void freeMouse()                   { m_mouse_anchored = false; SDL_SetRelativeMouseMode(SDL_FALSE); }

        /**
         * get window width and height
         */
        glm::vec2 getDims() { return m_dims; }

        /**
         * get last measured fps 
         */
        float getFPS() { return m_fps; }

        /**
         * set or get game speed 
         */
        float& game_speed() { return m_game_speed; }

        /**
         * frustrum definition
         */
        constexpr static float DefaultFrustrumMin = 0.1f;
        constexpr static float DefaultFrustrumMax = 2000.0f;
        
        void init(int argc, const char** argv, const char* name, u32 width, u32 height, float max_fps = 60);
        void destroy();

    private:

        int           m_argc;
        const char**  m_argv;
        std::string   m_exe_folder;
        
        glm::ivec2    m_dims;
        SDL_Window*   m_window { nullptr };
        SDL_GLContext m_context;
        FPSLimiter    m_fps_limiter;
        float         m_fps;
        float         m_game_speed { 1 };
        FBObject      m_main_fbo;
        glm::vec3     m_background_color { 0 };
        
        bool          m_running { true };
        
        glm::ivec2    m_mouse_pos;
        glm::ivec2    m_mouse_old_pos;
        glm::ivec2    m_mouse_delta;
        bool          m_mouse_anchored { false };
        glm::ivec2    m_mouse_anchor;
        bool          m_mouse_tracking { true };
    
        Gamepad       m_gamepad;
        const u8*     m_keys;

        TimeInterval  m_timer;

        enum class ProjectionType
        {
            Perspective,
            Orthographic
        } m_projection { ProjectionType::Perspective };

        float m_proj_width_over_height;
        float m_proj_left;
        float m_proj_right;
        float m_proj_down;
        float m_proj_up;
        float m_proj_near;
        float m_proj_far;
    };
};