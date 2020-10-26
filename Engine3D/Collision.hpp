#pragma once

#include <glm/glm.hpp>
#include "Shapes.hpp"

namespace Engine3D
{
	namespace Collision
	{
		//TODO: make all functions use this data structure as a return value
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

		Data RayVsPlane(const glm::vec3& pos1, const glm::vec3& dir,
					    const glm::vec3& pos2, const glm::vec3& nor);
		Data RayVsPlane(const Ray& ray, const Plane& plane);

		Data RayVsCylinder(const glm::vec3& pos1, const glm::vec3& dir,
						   const glm::vec3& pos2, const glm::vec2& dims);
		Data RayVsCylinder(const Ray& ray, const Cylinder& cylinder);

		Data CylinderVsCylinderSweep(const glm::vec3& pos1, const glm::vec2& dims1, const glm::vec3& velocity,
									 const glm::vec3& pos2, const glm::vec2& dims2);
		Data CylinderVsCylinderSweep(const Cylinder& c1, const glm::vec3& velocity, const Cylinder& c2);

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
		 * sweep ball vs triangle
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
		 * triangle vs triangle
		 *
		 */
		Data TriangleVsTriangle(const glm::vec3& t1_p1, const glm::vec3& t1_p2, const glm::vec3& t1_p3,
								const glm::vec3& t2_p1, const glm::vec3& t2_p2, const glm::vec3& t2_p3);
		Data TriangleVsTriangle(const Triangle& t1, const Triangle& t2);
	};
};