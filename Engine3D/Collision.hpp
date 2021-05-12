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

#include <glm/glm.hpp>
#include "Shapes.hpp"
#include "SceneObject.hpp"

namespace Engine3D
{
    namespace Collision
    {
        struct Data
        {
            Data() {}

            bool      occurred     { false };
            glm::vec3 displacement { 0 };

            explicit operator bool() const { return occurred; }
        };

        /**
         * box vs box
         *
         * \arg pos1  - 3D center position of the first box
         * \arg dims1 - width, height and depth of the first box
         * \arg pos2  - 3D center position of the second box
         * \arg dims2 - width, height and depth of the second box
         */
        Data BoxVsBox(const glm::vec3& pos1, const glm::vec3& dims1,
                      const glm::vec3& pos2, const glm::vec3& dims2);
        Data BoxVsBox(const Box& b1, const Box& b2);

        /**
         * ray vs box
         *
         * \arg pos1 - 3D position of the ray start
         * \arg dir  - 3D direction of the ray
         * \arg pos2 - 3D center position of the box
         * \arg dims - width, height and depth of the box
         */
        Data RayVsBox(const glm::vec3& pos1, const glm::vec3& dir,
                      const glm::vec3& pos2, const glm::vec3& dims);
        Data RayVsBox(const Ray& ray, const Box& box);

        /**
         * sweep box vs box
         *
         * \arg pos1     - 3D center position of the first box
         * \arg dims1    - width, height and depth of the first box
         * \arg velocity - velocity of the first box
         * \arg pos2     - 3D center position of the second box
         * \arg dims2    - width, height and depth of the second box
         */
        Data BoxVsBoxSweep(const glm::vec3& pos1, const glm::vec3& dims1, const glm::vec3& velocity,
                           const glm::vec3& pos2, const glm::vec3& dims2);
        Data BoxVsBoxSweep(const Box& b1, const glm::vec3& velocity, const Box& b2);

        /**
         * ball vs ball
         *
         * \arg pos1    - 3D center position of the first ball
         * \arg radius1 - radius of the first ball
         * \arg pos2    - 3D center position of the second ball
         * \arg radius2 - radius of the second ball
         */
        Data BallVsBall(const glm::vec3& pos1, const float& radius1,
                        const glm::vec3& pos2, const float& radius2);
        Data BallVsBall(const Ball& b1, const Ball& b2);

        /**
         * ray vs ball
         *
         * \arg pos1   - 3D position of the ray start
         * \arg dir    - 3D direction of the ray
         * \arg pos2   - 3D center position of the ball
         * \arg radius - radius of the ball
         */
        Data RayVsBall(const glm::vec3& pos1, const glm::vec3& dir,
                       const glm::vec3& pos2, const float& radius);
        Data RayVsBall(const Ray& ray, const Ball& ball);

        /**
         * sweep ball vs ball
         *
         * \arg pos1     - 3D center position of the first ball
         * \arg radius1  - radius of the first ball
         * \arg velocity - velocity of the first ball
         * \arg pos2     - 3D center position of the second ball
         * \arg radius2  - radius of the second ball
         */
        Data BallVsBallSweep(const glm::vec3& pos1, const float& radius1, const glm::vec3& velocity,
                             const glm::vec3& pos2, const float& radius2);
        Data BallVsBallSweep(const Ball& b1, const glm::vec3& velocity, const Ball& b2);

        /**
         * cylinder vs cylinder
         *
         * \arg pos1  - 3D center of the first cylinder
         * \arg dims1 - first value is the radius of the first cylinder and the second one is height
         * \arg pos2  - 3D center of the second cylinder
         * \arg dims2 - first value is the radius of the second cylinder and the second one is height
         */
        Data CylinderVsCylinder(const glm::vec3& pos1, const glm::vec2& dims1,
                                const glm::vec3& pos2, const glm::vec2& dims2);
        Data CylinderVsCylinder(const Cylinder& c1, const Cylinder& c2);

        /**
         * ray vs plane
         *
         * \arg pos1 - 3D position of the ray start
         * \arg dir  - 3D direction of the ray
         * \arg pos2 - 3D point on the plane
         * \arg nor  - 3D normal describing the plane
         */
        Data RayVsPlane(const glm::vec3& pos1, const glm::vec3& dir,
                        const glm::vec3& pos2, const glm::vec3& nor);
        Data RayVsPlane(const Ray& ray, const Plane& plane);

        /**
         * ray vs cylinder
         *
         * \arg pos1 - 3D position of the ray start
         * \arg dir  - 3D direction of the ray
         * \arg pos2 - 3D position of the cylinder
         * \arg dims - width and height of the cylinder
         */
        Data RayVsCylinder(const glm::vec3& pos1, const glm::vec3& dir,
                           const glm::vec3& pos2, const glm::vec2& dims);
        Data RayVsCylinder(const Ray& ray, const Cylinder& cylinder);

        /**
         * cylinder vs cylinder sweep
         *
         * \arg pos1     - 3D position of the first cylinder
         * \arg dims1    - width and height of the first cylinder
         * \arg velocity - velocity of the first cylinder
         * \arg pos2     - 3D position of the second cylinder
         * \arg dims2    - width and height of the second cylinder
         */
        Data CylinderVsCylinderSweep(const glm::vec3& pos1, const glm::vec2& dims1, const glm::vec3& velocity,
                                     const glm::vec3& pos2, const glm::vec2& dims2);
        Data CylinderVsCylinderSweep(const Cylinder& c1, const glm::vec3& velocity, const Cylinder& c2);

        /**
         * ray vs bean
         *
         * \arg pos1 - 3D position of the ray start
         * \arg dir  - 3D direction of the ray
         * \arg pos2 - 3D position of the bean
         * \arg dims - width and height of the bean
         */
        Data RayVsBean(const glm::vec3& pos1, const glm::vec3& dir,
                       const glm::vec3& pos2, const glm::vec2& dims);
        Data RayVsBean(const Ray& ray, const Bean& bean);

        /**
         * ball vs line
         *
         * \arg pos    - 3D center of the ball
         * \arg radius - radius of the ball
         * \arg p1     - start of the line
         * \arg p2     - end of the line
         */
        Data BallVsLine(const glm::vec3& pos, const float& radius,
                        const glm::vec3& p1, const glm::vec3& p2);
        Data BallVsLine(const Ball& ball, const Line& line);

        /**
         * sweep ball vs line
         *
         * \arg pos      - 3D center of the ball
         * \arg radius   - radius of the ball
         * \arg velocity - velocity of the ball
         * \arg p1       - start of the line
         * \arg p2       - end of the line
         */
        Data BallVsLineSweep(const glm::vec3& pos, const float& radius, const glm::vec3& velocity,
                             const glm::vec3& p1, const glm::vec3& p2);
        Data BallVsLineSweep(const Ball& ball, const glm::vec3& velocity, const Line& line);

        /**
         * ellipsoid vs line
         *
         * \arg pos  - 3D center of the ellipsoid
         * \arg dims - width, height and depth of the ellipsoid
         * \arg p1   - start of the line
         * \arg p2   - end of the line
         */
        Data EllipsoidVsLine(const glm::vec3& pos, const glm::vec3& dims,
                             const glm::vec3& p1, const glm::vec3& p2);
        Data EllipsoidVsLine(const Ellipsoid& ellipsoid, const Line& line);

        /**
         * ball vs triangle
         *
         * \arg pos        - 3D center of the ball
         * \arg radius     - radius of the ball
         * \arg p1, p2, p3 - triangle points
         */
        Data BallVsTriangle(const glm::vec3& pos, const float& radius,
                            const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
        Data BallVsTriangle(const Ball& ball, const Triangle& triangle);

        /**
         * ray vs triangle
         *
         * \arg pos - 3D position of the ray start
         * \arg dir - 3D direction of the ray
         * \arg p1  - first 3D point of the triangle
         * \arg p2  - second 3D point of the triangle
         * \arg p3  - third 3D point of the triangle
         */
        Data RayVsTriangle(const glm::vec3& pos, const glm::vec3& dir,
                           const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
        Data RayVsTriangle(const Ray& ray, const Triangle& triangle);

        /**
         * ball vs triangle sweep
         *
         * \arg pos        - 3D center of the ball
         * \arg radius     - radius of the ball
         * \arg velocity   - velocity of the ball
         * \arg p1, p2, p3 - triangle points
         */
        Data BallVsTriangleSweep(const glm::vec3& pos, const float& radius, const glm::vec3& velocity,
                                 const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
        Data BallVsTriangleSweep(const Ball& ball, const glm::vec3& velocity, const Triangle& triangle);

        /**
         * ellipsoid vs triangle
         *
         * \arg pos        - 3D center of the ellipsoid
         * \arg dims       - width, height and depth of the ellipsoid
         * \arg p1, p2, p3 - triangle points
         */
        Data EllipsoidVsTriangle(const glm::vec3& pos, const glm::vec3& dims,
                                 const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
        Data EllipsoidVsTriangle(const Ellipsoid& ellipsoid, const Triangle& triangle);

        /**
         * sweep ellipsoid vs triangle
         *
         * \arg pos        - 3D center of the ellipsoid
         * \arg dims       - width, height and depth of the ellipsoid
         * \arg velocity   - velocity of the ellipsoid
         * \arg p1, p2, p3 - triangle points
         */
        Data EllipsoidVsTriangleSweep(const glm::vec3& pos, const glm::vec3& dims, const glm::vec3& velocity,
                                       const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
        Data EllipsoidVsTriangleSweep(const Ellipsoid& ellipsoid, const glm::vec3& velocity, const Triangle& triangle);

        /**
         * plane vs plane
         *
         * \arg pos1 - 3D point on the first plane
         * \arg nor1 - 3D normal defining the first plane
         * \arg pos2 - 3D point on the second plane
         * \arg nor1 - 3D normal defining the second plane
         */
        Ray PlaneVsPlane(const glm::vec3& pos1, const glm::vec3& nor1,
                         const glm::vec3& pos2, const glm::vec3& nor2);
        Ray PlaneVsPlane(const Plane& plane1, const Plane& plane2);

        /**
         * triangle vs triangle
         *
         * \arg t1_p1 - first 3D point of the first triangle
         * \arg t1_p2 - second 3D point of the first triangle
         * \arg t1_p3 - third 3D point of the first triangle
         * \arg t2_p1 - first 3D point of the second triangle
         * \arg t2_p2 - second 3D point of the second triangle
         * \arg t2_p3 - third 3D point of the second triangle
         */
        Data TriangleVsTriangle(const glm::vec3& t1_p1, const glm::vec3& t1_p2, const glm::vec3& t1_p3,
                                const glm::vec3& t2_p1, const glm::vec3& t2_p2, const glm::vec3& t2_p3);
        Data TriangleVsTriangle(const Triangle& t1, const Triangle& t2);
        Ray LineVsLineMin(const glm::vec3& l1_p1, const glm::vec3& l1_p2, const glm::vec3& l2_p1, const glm::vec3& l2_p2);
        Ray LineVsLineMin(const Line& l1, const Line& l2);
        Data LineVsTriangle(const glm::vec3& l_p1, const glm::vec3& l_p2, const glm::vec3& t_p1, const glm::vec3& t_p2, const glm::vec3& t_p3);
        Data LineVsTriangle(const Line& l, const Triangle& t);
        Data TriangleVsTriangleSweep(const glm::vec3& t1_p1, const glm::vec3& t1_p2, const glm::vec3& t1_p3, const glm::vec3& velocity,
                                     const glm::vec3& t2_p1, const glm::vec3& t2_p2, const glm::vec3& t2_p3);
        Data TriangleVsTriangleSweep(const Triangle& t1, const glm::vec3& velocity, const Triangle& t2);
        /**
         * mesh vs mesh using separating axes theorem algorithm
         */
        Data MeshVsMeshSeparatingAxesTheorem(SceneObject* m1, SceneObject* m2);
        
        /**
         * mesh vs mesh
         */
        Data MeshVsMesh(SceneObject* m1, SceneObject* m2);
        Data MeshVsMeshSweep(SceneObject* m1, const glm::vec3& velocity, SceneObject* m2);
        Data MeshVsMeshPartial(SceneObject* m1, SceneObject* m2);
    };
};