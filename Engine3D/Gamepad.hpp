#pragma once

#include <string>
#include <unordered_map>
#include <SDL2/SDL.h>

#include "Types.hpp"

namespace Engine3D
{
    /**
     * setup friend class
     */
    class Game;
    
    /**
     * gamepad controller wrapper
     */
    class Gamepad
    {
    public:

        /**
         * enumeration of all button names
         */
        enum class ButtonName
        {
            A, B, X, Y,
            
            Back,
            Guide,
            
            DPadDown,
            DPadLeft,
            DPadRight,
            DPadUp,
            
            LeftShoulder,
            RightShoulder,
            
            LeftTrigger,
            RightTrigger,
            
            LeftAxisX,
            LeftAxisY,
            
            RightAxisX,
            RightAxisY,
            
            Start
        };

        /**
         * constructor
         */
        Gamepad() {}
        /**
         * destructor
         */
       ~Gamepad();
       
        /**
         * try to connect a controller to the system
         */
        bool use(u32 controller_num);
        
        /**
         * stop using controller
         */
        void unuse();
        
        /**
         * get button or axis state
         */
        int button(ButtonName button);
        
        /**
         * get active flag
         */
        bool active() { return m_active; }
       
    private:
    
        friend class Game;

        bool m_active { false };
        
        std::unordered_map<std::string, u8> m_button_mapping;
        std::unordered_map<std::string, u8> m_axes_mapping;
        
        u32 m_id;
        
        SDL_Joystick* m_joystick;
        
        s16* m_axes { nullptr };
        u32  m_num_axes { 0 };
        
        u8* m_buttons { nullptr };
        u32 m_num_buttons { 0 };
    };
};













