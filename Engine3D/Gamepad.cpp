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
#include "Gamepad.hpp"

#include <string>

namespace Engine3D
{
    /**
     * destructor
     */
    Gamepad::~Gamepad()
    {
        unuse();
    }
    
    /**
     * try to connect a controller to the system
     */
    bool Gamepad::use(u32 controller_num)
    {
        m_active = SDL_NumJoysticks() > 0 && static_cast<int>(controller_num) < SDL_NumJoysticks();
        
        if(!m_active)
        {
            //TODO: warning?
            return false;
        }

        //TODO: handle opening joystick better
        m_joystick = SDL_JoystickOpen(controller_num);
        
        // allocate space for button states
        m_num_axes    = SDL_JoystickNumAxes(m_joystick);
        m_num_buttons = SDL_JoystickNumButtons(m_joystick);
        
        m_axes    = new s16[m_num_axes];
        m_buttons = new u8[m_num_buttons];
        
        // get controller mapping
#if ENGINE3D_PLATFORM == WINDOWS
        SDL_GameController* gc = SDL_GameControllerOpen(controller_num);
        std::string mapping    = std::string(SDL_GameControllerMapping(gc));
        SDL_GameControllerClose(gc);
#else
        std::string mapping    = SDL_GameControllerMappingForGUID(SDL_JoystickGetGUID(m_joystick));
#endif

        u32 map_counter = 0;
        u32 map_start   = 0;
        
        // parse SDL joystick mapping
        for(u32 i = 0; i < mapping.size(); i++)
        {
            // start processing mapping after the first comma
            if(mapping[i] == ',') { map_counter++; } else { continue; }
            
            // skip guid and name
            if(map_counter <   3) { map_start = i + 1; continue; }
            
            std::string map_data = std::string(mapping.begin() + map_start, mapping.begin() + i);

            // extract button name and index
            std::string button      = std::string(map_data.begin(), map_data.begin() + map_data.find(':'));
            
            char map_category       = map_data[map_data.find(':') + 1];
            std::string map_value_s = std::string(map_data.begin() + map_data.find(':') + 2, map_data.end());
            
            u8 map_value;
            
            // skip hat decoding
            if(map_category == 'h')
            {
                map_start = i + 1;
                continue;
            } 
            else 
            {
                map_value = (u8)std::stoi(map_value_s);
            }
            
            // map the buttons
            switch(map_category) 
            {
                case 'b':
                {
                    m_button_mapping.insert(std::pair<std::string, u8>(button, map_value));
                    break;
                }
                case 'a':
                {
                    m_axes_mapping.insert(std::pair<std::string, u8>(button, map_value));
                    break;
                }
            }
            
            map_start = i + 1;
        }
        
        return true;
    }
    
    /**
     * stop using controller
     */
    void Gamepad::unuse()
    {
        if(m_active && m_joystick != nullptr)
        {
            if(m_axes    != nullptr) { delete[] m_axes;    } m_axes    = nullptr;
            if(m_buttons != nullptr) { delete[] m_buttons; } m_buttons = nullptr;
            
            SDL_JoystickClose(m_joystick);
            m_joystick = nullptr;
            m_active   = false;
        }
    }

    /**
     * get button or axis state
     */
    int Gamepad::button(ButtonName button)
    {
        static const char* button_translation[] = 
        {
            "a",
            "b",
            "x",
            "y",
            "back",
            "guide",
            "dpdown",
            "dpleft",
            "dpright",
            "dpup",
            "leftshoulder",
            "rightshoulder",
            "leftstick",
            "rightstick",
            "lefttrigger",
            "righttrigger",
            "leftx",
            "lefty",
            "ŕightx",
            "righty",
            "start",
        };
        
        // process joystick axes
        if(button >= ButtonName::LeftAxisX && button <= ButtonName::RightAxisY)
        {
            auto it = m_axes_mapping.find(button_translation[static_cast<u32>(button)]);
            
            if(it != m_axes_mapping.end())
            {
                return m_buttons[it->second];
            }
            
            return 0;
        }
        // process buttons
        else
        {
            auto it = m_button_mapping.find(button_translation[static_cast<u32>(button)]);
            
            if(it != m_button_mapping.end())
            {
                return m_buttons[it->second];
            }
            
            return 0;
        }
    }
};