#include "Collision.hpp"

#include <algorithm>
#include <cmath>

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine3D
{
	namespace Collision
	{
		/**
		 * box vs box
		 *
		 * \arg pos1  - 3D center position of the first box
		 * \arg dims1 - width, height and depth, of the first box
		 * \arg pos2  - 3D center position of the second box
		 * \arg dims2 - width, height and depth, of the second box
		 */
		Data BoxVsBox(const glm::vec3& pos1, const glm::vec3& dims1,
			          const glm::vec3& pos2, const glm::vec3& dims2)
		{
			Data res;

			//calculate delta and minimal width
			glm::vec3 delta        = pos2 - pos1;
			glm::vec3 delta_abs    = glm::abs(delta);
			glm::vec3 dims_limit   = dims1 / 2.0f + dims2 / 2.0f;

			//we won't be able to determine separation vector, if 2 objects perfecly overlap
			if (delta == glm::vec3(0))
			{
				res.occurred       = true;
				res.displacement.y = -dims_limit.y;
				return res;
			}

			//perform AABB check
			if(delta_abs.x <= dims_limit.x && 
			   delta_abs.y <= dims_limit.y &&
			   delta_abs.z <= dims_limit.z)
			{
				//calculate minimal translation vector
				glm::vec3 result = dims_limit - delta_abs;

				//change collision flag
				res.occurred = true;

				//choose x
				if (result.x < result.y && result.x < result.z)
				{
					res.displacement.x = -result.x * delta.x / delta_abs.x;
				}
				//choose y
				else if (result.y < result.z)
				{
					res.displacement.y = -result.y * delta.y / delta_abs.y;
				}
				//choose z
				else
				{
					res.displacement.z = -result.z * delta.z / delta_abs.z;
				}
			}

			return res;
		}
		Data BoxVsBox(const Box& b1, const Box& b2)
		{
			return BoxVsBox(b1.pos, b1.dims, b2.pos, b2.dims);
		}

		/**
		 * ray vs box
		 *
		 * \arg pos1 - 3D position of the ray start
		 * \arg dir  - 3D direction of the ray
		 * \arg pos2 - 3D center position of the box
		 * \arg dims - width, height and depth of the box
		 */
		Data RayVsBox(const glm::vec3& pos1, const glm::vec3& dir,
					  const glm::vec3& pos2, const glm::vec3& dims)
		{
			Data res;

			if (dir == glm::vec3(0))
			{
				return res;
			}

			glm::vec3 dir_normalized = glm::normalize(dir);

			//calculate near and far collision normalized offset
			glm::vec3 near_collision = (pos2 - dims / 2.0f - pos1) / dir_normalized;
			glm::vec3 far_collision  = (pos2 + dims / 2.0f - pos1) / dir_normalized;

			//sort the near and far intersection points so we can then apply collision rules
			if (near_collision.x > far_collision.x) { std::swap(near_collision.x, far_collision.x); }
			if (near_collision.y > far_collision.y) { std::swap(near_collision.y, far_collision.y); }
			if (near_collision.z > far_collision.z) { std::swap(near_collision.z, far_collision.z); }

			//apply not intersecting rule
			if (near_collision.x > far_collision.x ||
				near_collision.y > far_collision.y ||
				near_collision.z > far_collision.z)
			{
				return res;
			}

			//apply intersecting rule
			float near_hit_time = std::max(near_collision.x, std::max(near_collision.y, near_collision.z));
			float far_hit_time  = std::min(far_collision.x,  std::min(far_collision.y,  far_collision.z));

			//reject collision outside of the ray boundaries
			if (far_hit_time < 0.0f) { return res; }
			if (near_hit_time > 1.0f) { return res; }

			//calculate collision point
			res.occurred     = true;
			res.displacement = near_hit_time * dir_normalized;

			return res;
		}
		Data RayVsBox(const Ray& ray, const Box& box)
		{
			return RayVsBox(ray.pos, ray.dir, box.pos, box.dims);
		}

		/**
		 * sweep box vs box
		 *
		 * \arg pos1  - 3D center position of the first box
		 * \arg dims1 - width, height and depth, of the first box
		 * \arg pos2  - 3D center position of the second box
		 * \arg dims2 - width, height and depth, of the second box
		 */
		Data BoxVsBoxSweep(const glm::vec3& pos1, const glm::vec3& dims1, const glm::vec3& velocity,
			               const glm::vec3& pos2, const glm::vec3& dims2)
		{
			//enlarge the second box so we can directly check just ray vs box
			glm::vec3 dims2_enlarged = dims2 + dims1;

			return RayVsBox(pos1, velocity, pos2, dims2_enlarged);
		}
		Data BoxVsBoxSweep(const Box& b1, const glm::vec3& velocity, const Box& b2)
		{
			return BoxVsBoxSweep(b1.pos, b1.dims, velocity, b2.pos, b2.dims);
		}

		/**
		 * ball vs ball
		 *
		 * \arg pos1    - 3D center position of the first ball
		 * \arg radius1 - radius of the first ball
		 * \arg pos2    - 3D center position of the second ball
		 * \arg radius2 - radius of the second ball
		 */
		Data BallVsBall(const glm::vec3& pos1, const float& radius1,
			            const glm::vec3& pos2, const float& radius2)
		{
			Data res;

			//calculate delta
			glm::vec3 delta = pos1 - pos2;

			//if the distance between balls is smaller than sum of their radii, collision happened
			if (glm::dot(delta, delta) <= (radius1 + radius2) * (radius1 + radius2))
			{
				res.occurred     = true;
				res.displacement = glm::normalize(delta) * (radius1 + radius2 - glm::length(delta));
			}

			return res;
		}
		Data BallVsBall(const Ball& b1, const Ball& b2)
		{
			return BallVsBall(b1.pos, b1.radius, b2.pos, b2.radius);
		}

		/**
		 * ray vs ball
		 *
		 * \arg pos1   - 3D position of the ray start
		 * \arg dir    - 3D direction of the ray
		 * \arg pos2   - 3D center position of the ball
		 * \arg radius - radius of the ball
		 */
		Data RayVsBall(const glm::vec3& pos1, const glm::vec3& dir,
			           const glm::vec3& pos2, const float& radius)
		{
			//TODO: test this
			Data res;

			glm::vec3 delta = pos1 - pos2;

			float a = glm::dot(dir, dir);
			float b = 2.0f * glm::dot(delta, dir);
			float c = glm::dot(delta, delta) - radius * radius;

			float d = b * b - 4.0f * a * c;

			if (d < 0.0f)
			{
				return res;
			}

			float time1 = (-b - sqrtf(d)) / (2.0f * a);
			float time2 = (-b + sqrtf(d)) / (2.0f * a);

			if (time1 >= 0.0f && time1 <= 1.0f)
			{
				res.occurred = true;
				res.displacement = dir * time1;
			}
			else if (time2 >= 0.0f && time2 <= 1.0f)
			{
				res.occurred = true;
				res.displacement = dir * time2;
			}
			
			return res;
		}
		Data RayVsBall(const Ray& ray, const Ball& ball)
		{
			return RayVsBall(ray.pos, ray.dir, ball.pos, ball.radius);
		}

		/**
		 * sweep ball vs ball
		 *
		 * \arg pos1    - 3D center position of the first ball
		 * \arg radius1 - radius of the first ball
		 * \arg pos2    - 3D center position of the second ball
		 * \arg radius2 - radius of the second ball
		 */
		Data BallVsBallSweep(const glm::vec3& pos1, const float& radius1, const glm::vec3& velocity,
							 const glm::vec3& pos2, const float& radius2)
		{
			float enlarged_radius = radius1 + radius2;

			return RayVsBall(pos1, velocity, pos2, enlarged_radius);
		}
		Data BallVsBallSweep(const Ball& b1, const glm::vec3& velocity, const Ball& b2)
		{
			return BallVsBallSweep(b1.pos, b1.radius, velocity, b2.pos, b2.radius);
		}

		/**
		 * cylinder vs cylinder
		 *
		 * \arg pos1  - 3D center of the first cylinder
		 * \arg dims1 - first value is the radius of the first cylinder and the second one is height
		 * \arg pos2  - 3D center of the second cylinder
		 * \arg dims2 - first value is the radius of the second cylinder and the second one is height
		 */
		Data CylinderVsCylinder(const glm::vec3& pos1, const glm::vec2& dims1,
			                    const glm::vec3& pos2, const glm::vec2& dims2)
		{
			Data res;

			//calculate distance between cylinders separately for horizontal and vertical axis
			glm::vec2 delta_horizontal     = glm::vec2(pos1.x - pos2.x, pos1.z - pos2.z);
			float     delta_vertical       = pos1.y - pos2.y;
			float     delta_vertical_abs   = fabs(delta_vertical);
			float     delta_horizontal_len = glm::dot(delta_horizontal, delta_horizontal);

			//we won't be able to determine separation vector, if cylinders perfecly overlap
			if(delta_vertical == 0 && delta_horizontal == glm::vec2(0))
			{
				res.occurred       = true;
				res.displacement.y = (dims1.y / 2 + dims2.y / 2);
				return res;
			}

			//check horizontal and vertical overlap
			if((delta_horizontal_len < (dims1.x / 2 + dims2.x / 2) * (dims1.x / 2 + dims2.x / 2)) &&
			   (delta_vertical_abs   < dims1.y / 2 + dims2.y / 2))
			{
				delta_horizontal_len = sqrtf(delta_horizontal_len);

				//calculate minimal translation vector
				float delta_horizontal_displacement = (dims1.x / 2 + dims2.x / 2) - delta_horizontal_len;
				float delta_vertical_displacement   = (dims1.y / 2 + dims2.y / 2) - delta_vertical_abs;

				//change collision flag
				res.occurred = true;

				//choose horizontal plane
				if (delta_horizontal_displacement < delta_vertical_displacement)
				{
					delta_horizontal = glm::normalize(delta_horizontal) * delta_horizontal_displacement;

					res.displacement.x = delta_horizontal.x;
					res.displacement.z = delta_horizontal.y;
				}
				//choose vertical axis
				else
				{
					res.displacement.y = delta_vertical_displacement;
				}
			}

			return res;
		}
		Data CylinderVsCylinder(const Cylinder& c1, const Cylinder& c2)
		{
			return CylinderVsCylinder(c1.pos, c1.dims, c2.pos, c2.dims);
		}

		Data RayVsPlane(const glm::vec3& pos1, const glm::vec3& dir,
						const glm::vec3& pos2, const glm::vec3& nor)
		{
			Data res;

			float denominator = glm::dot(nor, dir);

			if (denominator == 0.0f)
			{
				return res;
			}

			float time = glm::dot(nor, pos2 - pos1) / denominator;

			if (time >= 0.0f && time <= 1.0f)
			{
				res.occurred     = true;
				res.displacement = dir * time;
			}

			return res;
		}
		Data RayVsPlane(const Ray& ray, const Plane& plane)
		{
			return RayVsPlane(ray.pos, ray.dir, plane.pos, plane.nor);
		}

		Data RayVsCylinder(const glm::vec3& pos1, const glm::vec3& dir,
						   const glm::vec3& pos2, const glm::vec2& dims)
		{
			Data res;

			std::printf("l: %f\n", glm::length(dir));
			
			//project the ray onto the cylinder from the top, so that we can check ray vs infinite height cylinder
			glm::vec2 delta2D = glm::vec2(pos1.x, pos1.z) - glm::vec2(pos2.x, pos2.z);
			glm::vec2 dir2D   = glm::vec2(dir.x, dir.z);
			static const glm::vec3 top_cap_normal = glm::vec3(0,  1, 0);
			static const glm::vec3 bot_cap_normal = glm::vec3(0, -1, 0);

			float a = glm::dot(dir2D, dir2D);
			float b = 2.0f * (dir2D.x * delta2D.x + dir2D.y * delta2D.y);
			float c = glm::dot(delta2D, delta2D) - dims.x * dims.x;

			float d = b * b - 4.0f * a * c;

			if (d < 0.0f)
			{
				return res;
			}

			float time1 = (-b - sqrtf(d)) / (2.0f * a);
			float time2 = (-b + sqrtf(d)) / (2.0f * a);

			float closest_time_horizontal = -1;

			if (time1 >= 0.0f && time1 <= 1.0f)
			{
				closest_time_horizontal = time1;
			}
			else if (time2 >= 0.0f && time2 <= 1.0f)
			{
				closest_time_horizontal = time2;
			}
			else
			{
				return res;
			}

			//we now know if there was a intersection with the horizontal part, now we need to check the caps
			glm::vec3 potential_intersection = pos1 + dir * closest_time_horizontal;

			//ray just hit the side of the cylinder
			if (std::abs(potential_intersection.y - pos2.y) < dims.y / 2.0f)
			{
				res.occurred     = true;
				res.displacement = dir * closest_time_horizontal;
			}

			//check if the ray hit one of the caps
			else
			{
				
				glm::vec3 top_cap_pos = pos2 - glm::vec3(0, dims.y / 2.0f, 0);
				glm::vec3 bot_cap_pos = pos2 + glm::vec3(0, dims.y / 2.0f, 0);

				Data top_cap_data = RayVsPlane(pos1, dir, top_cap_pos, top_cap_normal);
				Data bot_cap_data = RayVsPlane(pos1, dir, bot_cap_pos, bot_cap_normal);

				if (top_cap_data.occurred)
				{
					if (bot_cap_data.occurred)
					{
						if (glm::dot(top_cap_data.displacement, top_cap_data.displacement) < glm::dot(bot_cap_data.displacement, bot_cap_data.displacement))
						{
							glm::vec3 top_cap_ray_delta = (pos1 + top_cap_data.displacement) - top_cap_pos;

							if (glm::dot(top_cap_ray_delta, top_cap_ray_delta) <= dims.x * dims.x)
							{
								res = bot_cap_data;
							}
						}
						else
						{
							glm::vec3 bot_cap_ray_delta = (pos1 + bot_cap_data.displacement) - bot_cap_pos;

							if (glm::dot(bot_cap_ray_delta, bot_cap_ray_delta) <= dims.x * dims.x)
							{
								res = bot_cap_data;
							}
						}
					}
					else
					{
						glm::vec3 top_cap_ray_delta = (pos1 + top_cap_data.displacement) - top_cap_pos;

						if (glm::dot(top_cap_ray_delta,top_cap_ray_delta) <= dims.x * dims.x)
						{
							res = bot_cap_data;
						}
					}
				}
				else if (bot_cap_data.occurred)
				{
					glm::vec3 bot_cap_ray_delta = (pos1 + bot_cap_data.displacement) - bot_cap_pos;

					if (glm::dot(bot_cap_ray_delta, bot_cap_ray_delta) <= dims.x * dims.x)
					{
						res = bot_cap_data;
					}
				}
			}
			
			return res;
		}
		Data RayVsCylinder(const Ray& ray, const Cylinder& cylinder)
		{
			return RayVsCylinder(ray.pos, ray.dir, cylinder.pos, cylinder.dims);
		}

		Data CylinderVsCylinderSweep(const glm::vec3& pos1, const glm::vec2& dims1, const glm::vec3& velocity,
									 const glm::vec3& pos2, const glm::vec2& dims2)
		{
			glm::vec2 enlarged_dims = dims1 + dims2;

			return RayVsCylinder(pos1, velocity, pos2, enlarged_dims);
		}
		Data CylinderVsCylinderSweep(const Cylinder& c1, const glm::vec3& velocity, const Cylinder& c2)
		{
			return CylinderVsCylinderSweep(c1.pos, c1.dims, velocity, c2.pos, c2.dims);
		}

		Data RayVsBean(const glm::vec3& pos1, const glm::vec3& dir,
					   const glm::vec3& pos2, const glm::vec2& dims)
		{
			glm::vec3 first_cap_pos  = pos2 + glm::vec3(0, dims.y / 2.0f, 0);
			glm::vec3 second_cap_pos = pos2 - glm::vec3(0, dims.y / 2.0f, 0);

			std::vector<Data> results = 
			{
				RayVsCylinder(pos1, dir, pos2, dims),
				RayVsBall(pos1, dir, first_cap_pos, dims.x),
				RayVsBall(pos1, dir, second_cap_pos, dims.x)
			};

			std::printf("c: %s, %f\n", results[0] ? "true" : "false", glm::length(results[0].displacement));
			std::printf("t: %s, %f\n", results[1] ? "true" : "false", glm::length(results[1].displacement));
			std::printf("b: %s, %f\n", results[2] ? "true" : "false", glm::length(results[2].displacement));

			std::sort(results.begin(), results.end(), [](const Data& d1, const Data& d2) -> bool 
			{ 
				float add1 = d1.occurred ? 0.0f : std::numeric_limits<float>::infinity();
				float add2 = d2.occurred ? 0.0f : std::numeric_limits<float>::infinity();

				return add1 + glm::dot(d1.displacement, d1.displacement) < add2 + glm::dot(d2.displacement, d2.displacement);
			});

			return results[0];
		}
		Data RayVsBean(const Ray& ray, const Bean& bean)
		{
			return RayVsBean(ray.pos, ray.dir, bean.pos, bean.dims);
		}

		/**
		 * ball vs line
		 *
		 * \arg pos    - 3D center of the ball
		 * \arg radius - radius of the ball
		 * \arg p1     - start of the line
		 * \arg p2     - end of the line
		 */
		Data BallVsLine(const glm::vec3& pos, const float& radius,
			            const glm::vec3& p1, const glm::vec3& p2)
		{
			Data res;

			//calculate deltas for each point and project the balls' center onto the line
			glm::vec3 start_delta = p1 - pos;
			glm::vec3 end_delta   = p2 - pos;
			glm::vec3 line_delta  = p2 - p1;
			glm::vec3 line_dir    = glm::normalize(line_delta);
			float     projection  = glm::dot(line_dir, start_delta);

			//if the position of the ball was projected inside the line
			if (projection < 0 && projection * projection < glm::dot(line_delta, line_delta))
			{
				//calculate minimal line to position vector
				line_dir = glm::cross(line_delta, start_delta);
				line_dir = glm::normalize(glm::cross(line_dir, line_delta));

				//check length of the vector against the balls' radius
				float delta = fabs(glm::dot(line_dir, start_delta));
				if (delta < radius)
				{
					res.occurred     = true;
					res.displacement = -line_dir * (radius - delta);
				}
			}
			//projection is outside of the line, check corners
			else
			{
				//check start corner
				if (glm::dot(start_delta, start_delta) < radius * radius)
				{
					res.occurred     = true;
					res.displacement = glm::normalize(-start_delta) * (radius - glm::length(start_delta));
				}

				//check end corner
				if (glm::dot(end_delta, end_delta) < radius * radius)
				{
					res.occurred      = true;
					res.displacement += glm::normalize(-end_delta) * (radius - glm::length(end_delta));
				}
			}

			return res;
		}
		Data BallVsLine(const Ball& ball, const Line& line)
		{
			return BallVsLine(ball.pos, ball.radius, line.p1, line.p2);
		}

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
							 const glm::vec3& p1, const glm::vec3& p2)
		{
			glm::vec3 line = p2 - p1;

			float inv_angle = glm::angle(glm::vec3(0, 1, 0), glm::normalize(line));
			glm::vec3 angle_dir;
			if(inv_angle < 0.1 || inv_angle > M_PI - 0.1)
			{
				angle_dir = glm::vec3(1, 0, 0);
			}
			else
			{
				angle_dir = glm::cross(glm::vec3(0, 1, 0), line);
			}

			float line_len = glm::length(line);

			glm::vec3 bean_pos  = glm::vec3(0, line_len / 2.0f, 0);
			glm::vec2 bean_dims = glm::vec2(radius, line_len);

			glm::vec3 ray_start = glm::rotate(pos - p1,              -inv_angle, angle_dir); 
			glm::vec3 ray_end   = glm::rotate((pos + velocity) - p1, -inv_angle, angle_dir);
			glm::vec3 ray_dir   = ray_end - ray_start;

			Data res = RayVsBean(ray_start, ray_dir, bean_pos, bean_dims);

			if(res)
			{
				res.displacement = glm::rotate(res.displacement, inv_angle, angle_dir);
			}

			return res;
		}
		Data BallVsLineSweep(const Ball& ball, const glm::vec3& velocity, const Line& line)
		{
			return BallVsLineSweep(ball.pos, ball.radius, velocity, line.p1, line.p2);
		}

		/**
		 * ellipsoid vs line
		 *
		 * \arg pos  - 3D center of the ellipsoid
		 * \arg dims - width, height and depth, of the ellipsoid
		 * \arg p1   - start of the line
		 * \arg p2   - end of the line
		 */
		Data EllipsoidVsLine(const glm::vec3& pos, const glm::vec3& dims,
			                 const glm::vec3& p1, const glm::vec3& p2)
		{
			Data res;

			//same as Ball vs Line, but we normalize the space so that the ellipsoid will become a ball with radius of 1
			//we do that by considering the ellipsoid's center as center of the new space and divide the point's coordinates by the ellispsoid's dims
			glm::vec3 translated_start_delta = (p1 - pos) / dims;
			glm::vec3 translated_end_delta   = (p2 - pos) / dims;

			glm::vec3 translated_p1 = translated_start_delta + pos;
			glm::vec3 translated_p2 = translated_end_delta   + pos;

			//calculate deltas for each point and project the ball's center onto the line
			glm::vec3 translated_line_delta  = (translated_p2) - (translated_p1);
			glm::vec3 line_dir = glm::normalize(translated_line_delta);
			float     projection = glm::dot(line_dir, translated_start_delta);

			//if the position of the ball was projected inside the line
			if (projection < 0 && projection * projection < glm::dot(translated_line_delta, translated_line_delta))
			{
				//calculate minimal line to position vector
				line_dir = glm::cross(translated_line_delta, translated_start_delta);
				line_dir = glm::normalize(glm::cross(line_dir, translated_line_delta));

				//check length of the vector against the ball's radius
				float delta = fabs(glm::dot(line_dir, translated_start_delta));
				if (delta < 1.0f)
				{
					res.occurred     = true;
					res.displacement = -line_dir * (1.0f - delta);
				}
			}
			//projection is outside of the line, check corners
			else
			{
				//check start corner
				if (glm::dot(translated_start_delta, translated_start_delta) < 1.0f * 1.0f)
				{
					res.occurred     = true;
					res.displacement = glm::normalize(-translated_start_delta) * (1.0f - glm::length(translated_start_delta));
				}

				//check end corner
				if (glm::dot(translated_end_delta, translated_end_delta) < 1.0f * 1.0f)
				{
					res.occurred      = true;
					res.displacement += glm::normalize(-translated_end_delta) * (1.0f - glm::length(translated_end_delta));
				}
			}

			return res;
		}
		Data EllipsoidVsLine(const Ellipsoid& ellipsoid, const Line& line)
		{
			return EllipsoidVsLine(ellipsoid.pos, ellipsoid.dims, line.p1, line.p2);
		}

		/**
		 * ball vs triangle
		 *
		 * \arg pos        - 3D center of the ball
		 * \arg radius     - radius of the ball
		 * \arg p1, p2, p3 - triangle points
		 */
		Data BallVsTriangle(const glm::vec3& pos, const float& radius,
			                const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
		{
			Data res;

			//calculate barycentric coordinates
			auto barycentric = [](const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p) -> glm::vec3
			{
				glm::vec3 d0 = b - a;
				glm::vec3 d1 = c - a;

				glm::vec3 d2 = p - a;
				float d00 = glm::dot(d0, d0);
				float d01 = glm::dot(d0, d1);
				float d11 = glm::dot(d1, d1);
				float d20 = glm::dot(d2, d0);
				float d21 = glm::dot(d2, d1);
				float denom = d00 * d11 - d01 * d01;

				float v = (d11 * d20 - d01 * d21) / denom;
				float w = (d00 * d21 - d01 * d20) / denom;

				return glm::vec3(v, w, 1.0 - v - w);
			};

			glm::vec3 bar = barycentric(p1, p2, p3, pos);

			//use barycentric coordinates to project the ball position onto the triangle plane and check if we are inside the triangle
			if (bar.x >= 0 && bar.x <= 1 &&
				bar.y >= 0 && bar.y <= 1 &&
				bar.z >= 0 && bar.z <= 1)
			{
				//calculate minimal plane to position vector
				glm::vec3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
				float delta = glm::dot(normal, p1 - pos);

				//check the vector against the ball's radius
				if (fabs(delta) < radius)
				{
					if (delta == 0)
					{
						res.occurred     = true;
						res.displacement = -normal * (radius);
					}
					else
					{
						res.occurred     = true;
						float amount     = ((radius - fabs(delta)) * delta / fabs(delta));
						res.displacement = -normal * (amount);
					}
				}
			}
			//we are outside of the triangle, check edges of the triangle
			else
			{
				res = BallVsLine(pos, radius, p1, p2);
				if (res.occurred == false)
				{
					res = BallVsLine(pos, radius, p2, p3);
					if (res.occurred == false)
					{
						res = BallVsLine(pos, radius, p3, p1);
					}
				}
			}
			
			return res;
		}
		Data BallVsTriangle(const Ball& ball, const Triangle& triangle)
		{
			return BallVsTriangle(ball.pos, ball.radius, triangle.p1, triangle.p2, triangle.p3);
		}

		/**
		 * sweep ball vs triangle
		 *
		 * \arg pos        - 3D center of the ball
		 * \arg radius     - radius of the ball
		 * \arg velocity   - velocity of the ball
		 * \arg p1, p2, p3 - triangle points
		 */
		Data BallVsTriangleSweep(const glm::vec3& pos, const float& radius, const glm::vec3& velocity,
								 const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
		{
			//TODO:
			Data res;

			return res;
		}
		Data BallVsTriangleSweep(const Ball& ball, const glm::vec3& velocity, const Triangle& triangle)
		{
			return BallVsTriangleSweep(ball.pos, ball.radius, velocity, triangle.p1, triangle.p2, triangle.p3);
		}

		/**
		 * ellipsoid vs triangle
		 *
		 * \arg pos        - 3D center of the ellipsoid
		 * \arg dims       - width, height and depth of the ellipsoid
		 * \arg p1, p2, p3 - triangle points
		 */
		Data EllipsoidVsTriangle(const glm::vec3& pos, const glm::vec3& dims,
			                     const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
		{
			Data res;

			//same as Ball vs Triangle, but we normalize the space so that the ellipsoid will become a ball with radius of 1
			//we do that by considering the ellipsoid's center as center of the new space and divide the point's coordinates by ellispsoid's dims
			glm::vec3 translated_p1_delta = (p1 - pos) / dims;
			glm::vec3 translated_p2_delta = (p2 - pos) / dims;
			glm::vec3 translated_p3_delta = (p3 - pos) / dims;

			glm::vec3 translated_p1 = translated_p1_delta + pos;
			glm::vec3 translated_p2 = translated_p2_delta + pos;
			glm::vec3 translated_p3 = translated_p3_delta + pos;

			//calculate barycentric coordinates
			auto barycentric = [](const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p) -> glm::vec3
			{
				glm::vec3 d0 = b - a;
				glm::vec3 d1 = c - a;

				glm::vec3 d2 = p - a;
				float d00 = glm::dot(d0, d0);
				float d01 = glm::dot(d0, d1);
				float d11 = glm::dot(d1, d1);
				float d20 = glm::dot(d2, d0);
				float d21 = glm::dot(d2, d1);
				float denom = d00 * d11 - d01 * d01;

				float v = (d11 * d20 - d01 * d21) / denom;
				float w = (d00 * d21 - d01 * d20) / denom;

				return glm::vec3(v, w, 1.0 - v - w);
			};

			glm::vec3 bar = barycentric(translated_p1, translated_p2, translated_p3, pos);

			//use barycentric coordinates to project the ball position onto the triangle plane and check if we are inside of the triangle
			if (bar.x >= 0 && bar.x <= 1 &&
				bar.y >= 0 && bar.y <= 1 &&
				bar.z >= 0 && bar.z <= 1)
			{
				//calculate minimal plane to position vector
				glm::vec3 normal = glm::normalize(glm::cross(translated_p2 - translated_p1, translated_p3 - translated_p1));
				float delta = glm::dot(normal, translated_p1_delta);

				//check the vector against the ball's radius
				if (fabs(delta) < 1.0f)
				{
					if (delta == 0)
					{
						res.occurred     = true;
						res.displacement = -normal * (1.0f);
					}
					else
					{
						res.occurred     = true;
						float amount     = ((1.0f - fabs(delta)) * delta / fabs(delta));
						res.displacement = -normal * (amount);
					}
				}
			}
			//we are outside of the triangle, check edges of the triangle
			else
			{
				res = EllipsoidVsLine(pos, dims, p1, p2);
				if (res.occurred == false)
				{
					res = EllipsoidVsLine(pos, dims, p2, p3);
					if (res.occurred == false)
					{
						res = EllipsoidVsLine(pos, dims, p3, p1);
					}
				}
			}

			return res;
		}
		Data EllipsoidVsTriangle(const Ellipsoid& ellipsoid, const Triangle& triangle)
		{
			return EllipsoidVsTriangle(ellipsoid.pos, ellipsoid.dims, triangle.p1, triangle.p2, triangle.p3);
		}

		/**
		 * sweep ellipsoid vs triangle
		 *
		 * \arg pos        - 3D center of the ellipsoid
		 * \arg dims       - width, height and depth of the ellipsoid
		 * \arg velocity   - velocity of the ellipsoid
		 * \arg p1, p2, p3 - triangle points
		 */
		Data EllipsoidVsTriangleSweep(const glm::vec3& pos, const glm::vec3& dims, const glm::vec3& velocity,
			const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
		{
			//TODO:
			Data res;

			return res;
		}
		Data EllipsoidVsTriangleSweep(const Ellipsoid& ellipsoid, const glm::vec3& velocity, const Triangle& triangle)
		{
			return EllipsoidVsTriangleSweep(ellipsoid.pos, ellipsoid.dims, velocity, triangle.p1, triangle.p2, triangle.p3);
		}

		Ray PlaneVsPlane(const glm::vec3& pos1, const glm::vec3& nor1,
					     const glm::vec3& pos2, const glm::vec3& nor2)
		{
			Ray res;

			glm::vec3 n = glm::cross(nor1, nor2);

			float a1 = nor1.x;
			float b1 = nor1.y;
			float c1 = nor1.z;
			float d1 = nor1.x * pos1.x + nor1.y * pos1.y + nor1.z * pos1.z;

			float a2 = nor2.x;
			float b2 = nor2.y;
			float c2 = nor2.z;
			float d2 = nor2.x * pos2.x + nor2.y * pos2.y + nor2.z * pos2.z;

			if(n.x != 0.0f)
			{
				res.pos = glm::vec3(0, 
									(c2 * d1 - c1 * d2) / (b2 * c1 - b1 * c2), 
					                (b2 * d1 - b1 * d2) / (b1 * c2 - b2 * c1)) / n.x;
			}
			else if(n.y != 0.0f)
			{
				res.pos = glm::vec3((c2 * d1 - c1 * d2) / (a2 * c1 - a1 * c2), 
									0,
					                (a2 * d1 - a1 * d2) / (a1 * c2 - a2 * c1)) / n.y;
			}
			else if(n.z != 0.0f)
			{
				res.pos = glm::vec3((b1 * d2 - b2 * d1) / (a1 * b2 - a2 * b1), 
					                (a2 * d1 - a1 * d2) / (a1 * b2 - a2 * b1), 
					                0) / n.z;
			}

			res.dir = glm::normalize(n);

			return res;
		}
		Ray PlaneVsPlane(const Plane& plane1, const Plane& plane2)
		{
			return PlaneVsPlane(plane1.pos, plane1.nor, plane2.pos, plane2.nor);
		}

		Data TriangleVsTriangle(const glm::vec3& t1_p1, const glm::vec3& t1_p2, const glm::vec3& t1_p3,
								const glm::vec3& t2_p1, const glm::vec3& t2_p2, const glm::vec3& t2_p3)
		{
			Data res;

			glm::vec3 plane1_nor = glm::cross(t1_p2 - t1_p1, t1_p3 - t1_p1);
			glm::vec3 plane2_nor = glm::cross(t2_p2 - t2_p1, t2_p3 - t2_p1);

			Ray intersection_line = PlaneVsPlane(t1_p1, plane1_nor, t2_p1, plane2_nor);

			bool t1_p1s = glm::dot(plane2_nor, t1_p1 - intersection_line.pos) > 0; std::printf("t1p1: %s\n", t1_p1s ? "true" : "false");
			bool t1_p2s = glm::dot(plane2_nor, t1_p2 - intersection_line.pos) > 0; std::printf("t1p2: %s\n", t1_p2s ? "true" : "false");
			bool t1_p3s = glm::dot(plane2_nor, t1_p3 - intersection_line.pos) > 0; std::printf("t1p3: %s\n", t1_p3s ? "true" : "false");

			bool t2_p1s = glm::dot(plane1_nor, t2_p1 - intersection_line.pos) > 0; std::printf("t2p1: %s\n", t2_p1s ? "true" : "false");
			bool t2_p2s = glm::dot(plane1_nor, t2_p2 - intersection_line.pos) > 0; std::printf("t2p2: %s\n", t2_p2s ? "true" : "false");
			bool t2_p3s = glm::dot(plane1_nor, t2_p3 - intersection_line.pos) > 0; std::printf("t2p3: %s\n", t2_p3s ? "true" : "false");

			if(( t1_p1s &&  t1_p2s &&  t1_p3s) ||
			   (!t1_p1s && !t1_p2s && !t1_p3s))
			{
				return res;
			}

			if(( t2_p1s &&  t2_p2s &&  t2_p3s) ||
			   (!t2_p1s && !t2_p2s && !t2_p3s))
			{
				return res;
			}

			res.occurred = true;
			
			return res;
		}
		Data TriangleVsTriangle(const Triangle& t1, const Triangle& t2)
		{
			return TriangleVsTriangle(t1.p1, t1.p2, t1.p3, t2.p1, t2.p2, t2.p3);
		}
	};
};