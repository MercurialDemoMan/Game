#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Macros.hpp"

namespace Engine3D
{
    /**
     * structure for calculating view and rotation matrix
     */
    class Camera
    {
    public:
        
        /**
        ¨* constructors
        */
        Camera(){}
        Camera(glm::vec3 pos) : m_pos(pos) {}
        Camera(glm::vec3 pos, glm::vec3 dir) : m_pos(pos), m_dir(glm::normalize(dir)) {}
        
        /**
         * getters
         */
        const glm::vec3& getPos() const { return m_pos; }
        const glm::vec3& getDir() const { return m_dir; }
        /**
         * setters
         */
        void setPos(glm::vec3& pos)  { m_rot_needs_update = m_view_needs_update = true; m_pos = pos; }
        void setPos(glm::vec3&& pos) { m_rot_needs_update = m_view_needs_update = true; m_pos = pos; }
        void setX(float x)           { m_rot_needs_update = m_view_needs_update = true; m_pos.x = x; }
        void setY(float y)           { m_rot_needs_update = m_view_needs_update = true; m_pos.y = y; }
        void setZ(float z)           { m_rot_needs_update = m_view_needs_update = true; m_pos.z = z; }
        void addPos(glm::vec3& pos)  { m_rot_needs_update = m_view_needs_update = true; m_pos += pos; }
        void addPos(glm::vec3&& pos) { m_rot_needs_update = m_view_needs_update = true; m_pos += pos; }
        void setDir(glm::vec3& dir)  { m_rot_needs_update = m_view_needs_update = true; m_dir = glm::normalize(dir); }
        void setDir(glm::vec3&& dir) { m_rot_needs_update = m_view_needs_update = true; m_dir = glm::normalize(dir); }

        /**
         * rotate the camera around a axis
         */
        void rotate(glm::vec3& axis, float angle_in_rad);
        void rotate(glm::vec3&& axis, float angle_in_rad);
        
        /**
         * calculate the rotation matrix
         */
        const glm::mat4& getRotationMatrix();
        
        /**
         * calculate the view matrix
         */
        const glm::mat4& getViewMatrix();
        
        /**
         * access field of view
         */
        void  setFov(float fov_in_deg) { m_fov = fov_in_deg; }
        float getFov()                 const { return m_fov; }
        float getFovInRad()            const { return static_cast<float>(m_fov * 180.0f / M_PI); }
        
    private:
    
        glm::vec3 m_pos { 0 };
        glm::vec3 m_dir { 0, 0, 1 };
        
        float     m_fov { 60 };
        
        bool      m_rot_needs_update { true };
        glm::mat4 m_rot_matrix;
        bool      m_view_needs_update { true };
        glm::mat4 m_view_matrix;
    };
};