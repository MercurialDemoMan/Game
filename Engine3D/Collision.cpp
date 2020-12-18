#include "Collision.hpp"

#include <algorithm>
#include <cmath>

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine3D
{
    namespace Collision
    {
    	static glm::vec3 barycentric(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p)
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
        }

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

        /**
         * ray vs plane
         *
         * \arg pos1 - 3D position of the ray start
         * \arg dir  - 3D direction of the ray
         * \arg pos2 - 3D point on the plane
         * \arg nor  - 3D normal describing the plane
         */
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

        /**
         * ray vs cylinder
         *
         * \arg pos1 - 3D position of the ray start
         * \arg dir  - 3D direction of the ray
         * \arg pos2 - 3D position of the cylinder
         * \arg dims - width and height of the cylinder
         */
        Data RayVsCylinder(const glm::vec3& pos1, const glm::vec3& dir,
                           const glm::vec3& pos2, const glm::vec2& dims)
        {
            Data res;
            
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
                                     const glm::vec3& pos2, const glm::vec2& dims2)
        {
            glm::vec2 enlarged_dims = dims1 + dims2;

            return RayVsCylinder(pos1, velocity, pos2, enlarged_dims);
        }
        Data CylinderVsCylinderSweep(const Cylinder& c1, const glm::vec3& velocity, const Cylinder& c2)
        {
            return CylinderVsCylinderSweep(c1.pos, c1.dims, velocity, c2.pos, c2.dims);
        }
        /**
         * ray vs bean
         *
         * \arg pos1 - 3D position of the ray start
         * \arg dir  - 3D direction of the ray
         * \arg pos2 - 3D position of the bean
         * \arg dims - width and height of the bean
         */
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
        Data RayVsTriangle(const glm::vec3& pos, const glm::vec3& dir,
                           const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
        {
        	glm::vec3 plane_normal = glm::cross(p2 - p1, p3 - p1);
        	Data res = RayVsPlane(pos, dir, p1, plane_normal);

			if(!res)
			{
				return res;
			}        	

			glm::vec3 intersection = pos + res.displacement;

			glm::vec3 bar = barycentric(p1, p2, p3, intersection);

            if (bar.x >= 0 && bar.x <= 1 &&
                bar.y >= 0 && bar.y <= 1 &&
                bar.z >= 0 && bar.z <= 1)
            {
            	return res;
            }

            return {};
        }
        Data RayVsTriangle(const Ray& ray, const Triangle& triangle)
        {
        	return RayVsTriangle(ray.pos, ray.dir, triangle.p1, triangle.p2, triangle.p3);
        }
        /**
         * ball vs triangle sweep
         *
         * \arg pos        - 3D center of the ball
         * \arg radius     - radius of the ball
         * \arg velocity   - velocity of the ball
         * \arg p1, p2, p3 - triangle points
         */
        Data BallVsTriangleSweep(const glm::vec3& pos, const float& radius, const glm::vec3& velocity,
                                 const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
        {
        	glm::vec3 triangle_normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));

        	glm::vec3 top_triangle_p1 = p1 + triangle_normal * radius;
        	glm::vec3 top_triangle_p2 = p2 + triangle_normal * radius;
        	glm::vec3 top_triangle_p3 = p3 + triangle_normal * radius;

        	glm::vec3 bot_triangle_p1 = p1 - triangle_normal * radius;
        	glm::vec3 bot_triangle_p2 = p2 - triangle_normal * radius;
        	glm::vec3 bot_triangle_p3 = p3 - triangle_normal * radius;

            std::vector<Data> results = 
            {
                BallVsLineSweep(pos, radius, velocity, p1, p2),
                BallVsLineSweep(pos, radius, velocity, p2, p3),
                BallVsLineSweep(pos, radius, velocity, p3, p1),
                RayVsTriangle(pos, velocity, top_triangle_p1, top_triangle_p2, top_triangle_p3),
                RayVsTriangle(pos, velocity, bot_triangle_p1, bot_triangle_p2, bot_triangle_p3)
            };
            
            std::sort(results.begin(), results.end(), [](const Data& d1, const Data& d2) -> bool 
            { 
                float add1 = d1.occurred ? 0.0f : std::numeric_limits<float>::infinity();
                float add2 = d2.occurred ? 0.0f : std::numeric_limits<float>::infinity();

                return add1 + glm::dot(d1.displacement, d1.displacement) < add2 + glm::dot(d2.displacement, d2.displacement);
            });

            return results[0];
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
                                      const glm::vec3& p1,  const glm::vec3& p2,   const glm::vec3& p3)
        {
            Data res;

            glm::vec3 translated_p1_delta = (p1 - pos) / dims;
            glm::vec3 translated_p2_delta = (p2 - pos) / dims;
            glm::vec3 translated_p3_delta = (p3 - pos) / dims;

            glm::vec3 translated_p1 = translated_p1_delta + pos;
            glm::vec3 translated_p2 = translated_p2_delta + pos;
            glm::vec3 translated_p3 = translated_p3_delta + pos;

            glm::vec3 translated_velocity = velocity / dims;

            return BallVsTriangleSweep(pos, 1, translated_velocity, translated_p1, translated_p2, translated_p3);
        }
        Data EllipsoidVsTriangleSweep(const Ellipsoid& ellipsoid, const glm::vec3& velocity, const Triangle& triangle)
        {
            return EllipsoidVsTriangleSweep(ellipsoid.pos, ellipsoid.dims, velocity, triangle.p1, triangle.p2, triangle.p3);
        }

        /**
         * plane vs plane
         *
         * \arg pos1 - 3D point on the first plane
         * \arg nor1 - 3D normal defining the first plane
         * \arg pos2 - 3D point on the second plane
         * \arg nor1 - 3D normal defining the second plane
         */
        Ray PlaneVsPlane(const glm::vec3& pos1, const glm::vec3& nor1,
                         const glm::vec3& pos2, const glm::vec3& nor2)
        {
            Ray res;

            glm::vec3 n = glm::cross(nor1, nor2);

            float det = glm::dot(n, n);

            if(det != 0.0)
            {
                res.pos = ((glm::cross(n, nor2) * -glm::dot(pos1, nor1)) + 
                           (glm::cross(nor1, n) * -glm::dot(pos2, nor2))) / det;
                res.dir = n;
            }

            return res;
        }
        Ray PlaneVsPlane(const Plane& plane1, const Plane& plane2)
        {
            return PlaneVsPlane(plane1.pos, plane1.nor, plane2.pos, plane2.nor);
        }

        /**
         * triangle vs triangle
         *
         * \arg t1_p1 - first 3D point of the first triangle
         * \arg t1_p2 - second 3D point of the first triangle
         * \arg t1_p3 - third 3D point of the first triangle
         * \arg t2_p1 - first 3D point of the second triangle
         * \arg t2_p2 - second 3D point of the second triangle
         * \arg t2_p3 - third 3D point of the second triangle
         *
         * \source - web.stanford.edu/class/cs277/resources/papers/Moller1997b.pdf
         */
        Data TriangleVsTriangle(const glm::vec3& t1_p1, const glm::vec3& t1_p2, const glm::vec3& t1_p3,
                                const glm::vec3& t2_p1, const glm::vec3& t2_p2, const glm::vec3& t2_p3)
        {
            Data res;

            // construct infinite plane from the triangle 2
            Plane p2 { t2_p1, glm::normalize(glm::cross(t2_p2 - t2_p1, t2_p3 - t2_p1)) };

            // check on which side of the plane 2 vertices of triangle 1 are
            float t1_p1d = glm::dot(p2.nor, t1_p1) - glm::dot(p2.pos, p2.nor); bool t1_p1s = t1_p1d > 0;
            float t1_p2d = glm::dot(p2.nor, t1_p2) - glm::dot(p2.pos, p2.nor); bool t1_p2s = t1_p2d > 0;
            float t1_p3d = glm::dot(p2.nor, t1_p3) - glm::dot(p2.pos, p2.nor); bool t1_p3s = t1_p3d > 0;

            // if all triangle 1 vertices are on one side, there is no collision
            if(( t1_p1s  && t1_p2s  && t1_p3s) ||
               (!t1_p1s && !t1_p2s && !t1_p3s))
            {
                return res;
            }

            // construct infinity plane from the triange 1
            Plane p1 { t1_p1, glm::normalize(glm::cross(t1_p2 - t1_p1, t1_p3 - t1_p1)) };

            // check on which side of the plane 1 vertices of triangle 2 are
            float t2_p1d = glm::dot(p1.nor, t2_p1) - glm::dot(p1.pos, p1.nor); bool t2_p1s = t2_p1d > 0;
            float t2_p2d = glm::dot(p1.nor, t2_p2) - glm::dot(p1.pos, p1.nor); bool t2_p2s = t2_p2d > 0;
            float t2_p3d = glm::dot(p1.nor, t2_p3) - glm::dot(p1.pos, p1.nor); bool t2_p3s = t2_p3d > 0;

            // if all triangle 2 vertices are on one side, there is no collision
            if(( t2_p1s  && t2_p2s  && t2_p3s) ||
               (!t2_p1s && !t2_p2s && !t2_p3s))
            {
                return res;
            }

            // calculate intersection between planes
            Ray intersection = PlaneVsPlane(p1, p2);

            // project all vertices onto the insersertion line
            float t1_p1t = glm::dot(intersection.dir, t1_p1);
            float t1_p2t = glm::dot(intersection.dir, t1_p2);
            float t1_p3t = glm::dot(intersection.dir, t1_p3);

            float t2_p1t = glm::dot(intersection.dir, t2_p1);
            float t2_p2t = glm::dot(intersection.dir, t2_p2);
            float t2_p3t = glm::dot(intersection.dir, t2_p3);

            auto opposites = [](bool x, bool y)
            {
                return (x && !y) || (!x && y);
            };

            auto line_intersection = [](float t1, float t2, float d1, float d2)
            {
                return t1 + (t2 - t1) * (d1 / (d1 - d2));
            };

            // calculate intervals where triangles intersect the plane intersection
            float t1_int0;
            float t1_int1;

            float t2_int0;
            float t2_int1;

            if(opposites(t1_p1s, t1_p3s) && opposites(t1_p2s, t1_p3s))
            {
                t1_int0 = line_intersection(t1_p1t, t1_p3t, t1_p1d, t1_p3d);
                t1_int1 = line_intersection(t1_p2t, t1_p3t, t1_p2d, t1_p3d);
            }
            else if(opposites(t1_p1s, t1_p2s) && opposites(t1_p3s, t1_p2s))
            {
                t1_int0 = line_intersection(t1_p1t, t1_p2t, t1_p1d, t1_p2d);
                t1_int1 = line_intersection(t1_p3t, t1_p2t, t1_p3d, t1_p2d);
            }
            else if(opposites(t1_p2s, t1_p1s) && opposites(t1_p3s, t1_p1s))
            {
                t1_int0 = line_intersection(t1_p2t, t1_p1t, t1_p2d, t1_p1d);
                t1_int1 = line_intersection(t1_p3t, t1_p1t, t1_p3d, t1_p1d);
            }

            if(opposites(t2_p1s, t2_p3s) && opposites(t2_p2s, t2_p3s))
            {
                t2_int0 = line_intersection(t2_p1t, t2_p3t, t2_p1d, t2_p3d);
                t2_int1 = line_intersection(t2_p2t, t2_p3t, t2_p2d, t2_p3d);
            }
            else if(opposites(t2_p1s, t2_p2s) && opposites(t2_p3s, t2_p2s))
            {
                t2_int0 = line_intersection(t2_p1t, t2_p2t, t2_p1d, t2_p2d);
                t2_int1 = line_intersection(t2_p3t, t2_p2t, t2_p3d, t2_p2d);
            }
            else if(opposites(t2_p2s, t2_p1s) && opposites(t2_p3s, t2_p1s))
            {
                t2_int0 = line_intersection(t2_p2t, t2_p1t, t2_p2d, t2_p1d);
                t2_int1 = line_intersection(t2_p3t, t2_p1t, t2_p3d, t2_p1d);
            }

            if(t1_int0 > t1_int1) { std::swap(t1_int0, t1_int1); }
            if(t2_int0 > t2_int1) { std::swap(t2_int0, t2_int1); }

            // if intervals overlap, there is a collision
            if(t1_int0 > t2_int1 || t1_int1 < t2_int0)
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
        Ray LineVsLineMin(const glm::vec3& l1_p1, const glm::vec3& l1_p2, const glm::vec3& l2_p1, const glm::vec3& l2_p2)
        {
            glm::vec3 delta    = l1_p1 - l2_p1;

            glm::vec3 delta_l2 = l2_p2 - l2_p1;
            glm::vec3 delta_l1 = l1_p2 - l1_p1;

            float d1 = glm::dot(delta,    delta_l2);
            float d2 = glm::dot(delta_l2, delta_l1);
            float d3 = glm::dot(delta,    delta_l1);

            float d4 = glm::dot(delta_l2, delta_l2);
            float d5 = glm::dot(delta_l1, delta_l1);

            float denominator = d5 * d4 - d2 * d2;
            if (denominator == 0.0f)
            {
                return Ray(glm::vec3(0), glm::vec3(std::numeric_limits<float>::infinity()));
            }
            float numerator = d1 * d2 - d3 * d4;

            float  arg1 = numerator / denominator;
            float  arg2 = (d1 + d2 * (arg1)) / d4;

            return Ray(l1_p1 + arg1 * delta_l1, (l2_p1 + arg2 * delta_l2) - (l1_p1 + arg1 * delta_l1));
        }
        Ray LineVsLineMin(const Line& l1, const Line& l2)
        {
            return LineVsLineMin(l1.p1, l1.p2, l2.p1, l2.p2);
        }
        Data TriangleVsTriangleSweep(const glm::vec3& t1_p1, const glm::vec3& t1_p2, const glm::vec3& t1_p3, const glm::vec3& velocity,
                                     const glm::vec3& t2_p1, const glm::vec3& t2_p2, const glm::vec3& t2_p3)
        {
            
            Data res;

            // construct infinite plane from the triangle 2
            Plane p2 { t2_p1, glm::normalize(glm::cross(t2_p2 - t2_p1, t2_p3 - t2_p1)) };

            res = RayVsPlane(t1_p1, velocity, p2.pos, p2.nor);
            if (res)
            {
                glm::vec3 test = barycentric(t2_p1, t2_p2, t2_p3, t1_p1 + res.displacement);

                if (test.x >= 0.0f && test.x <= 1.0f &&
                    test.y >= 0.0f && test.y <= 1.0f &&
                    test.z >= 0.0f && test.z <= 1.0f)
                {
                    return res;
                }
                else
                {
                    res.occurred     = false;
                    res.displacement = glm::vec3(0);
                }
            }

            res = RayVsPlane(t1_p2, velocity, p2.pos, p2.nor);
            if (res)
            {
                glm::vec3 test = barycentric(t2_p1, t2_p2, t2_p3, t1_p2 + res.displacement);

                if (test.x >= 0.0f && test.x <= 1.0f &&
                    test.y >= 0.0f && test.y <= 1.0f &&
                    test.z >= 0.0f && test.z <= 1.0f)
                {
                    return res;
                }
                else
                {
                    res.occurred     = false;
                    res.displacement = glm::vec3(0);
                }
            }

            res = RayVsPlane(t1_p3, velocity, p2.pos, p2.nor);
            if (res)
            {
                glm::vec3 test = barycentric(t2_p1, t2_p2, t2_p3, t1_p3 + res.displacement);

                if (test.x >= 0.0f && test.x <= 1.0f &&
                    test.y >= 0.0f && test.y <= 1.0f &&
                    test.z >= 0.0f && test.z <= 1.0f)
                {
                    return res;
                }
                else
                {
                    res.occurred     = false;
                    res.displacement = glm::vec3(0);
                }
            }

            // construct infinite plane from the triangle 1
            Plane p1 { t1_p1, glm::normalize(glm::cross(t1_p2 - t1_p1, t1_p3 - t1_p1)) };

            res = RayVsPlane(t2_p1, velocity, p1.pos, p1.nor);
            if (res)
            {
                glm::vec3 test = barycentric(t1_p1, t1_p2, t1_p3, t2_p1 + res.displacement);

                if (test.x >= 0.0f && test.x <= 1.0f &&
                    test.y >= 0.0f && test.y <= 1.0f &&
                    test.z >= 0.0f && test.z <= 1.0f)
                {
                    res.displacement = -res.displacement;
                    return res;
                }
                else
                {
                    res.occurred     = false;
                    res.displacement = glm::vec3(0);
                }
            }

            res = RayVsPlane(t2_p2, velocity, p1.pos, p1.nor);
            if (res)
            {
                glm::vec3 test = barycentric(t1_p1, t1_p2, t1_p3, t2_p2 + res.displacement);

                if (test.x >= 0.0f && test.x <= 1.0f &&
                    test.y >= 0.0f && test.y <= 1.0f &&
                    test.z >= 0.0f && test.z <= 1.0f)
                {
                    res.displacement = -res.displacement;
                    return res;
                }
                else
                {
                    res.occurred = false;
                    res.displacement = glm::vec3(0);
                }
            }

            res = RayVsPlane(t2_p3, velocity, p1.pos, p1.nor);
            if (res)
            {
                glm::vec3 test = barycentric(t1_p1, t1_p2, t1_p3, t2_p3 + res.displacement);

                if (test.x >= 0.0f && test.x <= 1.0f &&
                    test.y >= 0.0f && test.y <= 1.0f &&
                    test.z >= 0.0f && test.z <= 1.0f)
                {
                    res.displacement = -res.displacement;
                    return res;
                }
                else
                {
                    res.occurred = false;
                    res.displacement = glm::vec3(0);
                }
            }

            Ray r1_start = LineVsLineMin(t1_p1, t1_p2, t2_p1, t2_p2);
            Ray r1_end   = LineVsLineMin(t1_p1 + velocity, t1_p2, t2_p1 + velocity, t2_p2);

            if (glm::dot(t2_p2 - t2_p1, r1_start.pos) > 0 != glm::dot(t2_p2 - t2_p1, r1_end.pos) > 0)
            {
                res.occurred     = true;
                res.displacement = r1_start.dir;
                return res;
            }

            return res;
        }
        Data TriangleVsTriangleSweep(const Triangle& t1, const glm::vec3& velocity, const Triangle& t2)
        {
            return TriangleVsTriangleSweep(t1.p1, t1.p2, t1.p3, velocity, t2.p1, t2.p2, t2.p3);
        }
        
        /**
         * mesh vs mesh using separating axes theorem algorithm
         */
        Data MeshVsMeshSeparatingAxesTheorem(SceneObject* m1, SceneObject* m2)
        {
            Data res;

            //variables for calculation of minimal translation vector
            float min_overlap = std::numeric_limits<float>::max();
            glm::vec3 min_trans_vec = glm::vec3(0);

            glm::vec3 delta = m2->pos() - m1->pos();

            //extract vertices
            const std::vector<Vertex>& m1_vertices = *m1->vertices();
            const std::vector<Vertex>& m2_vertices = *m2->vertices();

            //for every triangle in mesh 1
            for(u32 i = 0; i < m1_vertices.size(); i += 3)
            {
                //calculate triangle normal
                glm::vec3 t_normal = glm::normalize(m1->getVertex(i).value().nor);
        
                float projection;

                //save bounds of the projected mesh
                float m1_min;
                float m1_max;

                //project mesh 1 onto the triangle normal
                for(u32 j = 0; j < m1_vertices.size(); j++)
                {
                    projection = glm::dot(m1->getVertex(j).value().pos, t_normal);

                    if(j == 0)
                    {
                        m1_min = m1_max = projection;
                    }
                    else
                    {
                        if(projection < m1_min) { m1_min = projection; }
                        if(projection > m1_max) { m1_max = projection; }
                    }
                }

                //save bounds of the projected mesh
                float m2_min;
                float m2_max;

                //project mesh 2 onto the triangle normal
                for(u32 j = 0; j < m2_vertices.size(); j++)
                {
                    projection = glm::dot(m2->getVertex(j).value().pos, t_normal);

                    if(j == 0)
                    {
                        m2_min = m2_max = projection;
                    }
                    else
                    {
                        if(projection < m2_min) { m2_min = projection; }
                        if(projection > m2_max) { m2_max = projection; }
                    }
                }

                //if there is a gap between meshes -> no collision
                if(!(m1_min <= m2_max && m2_min <= m1_max))
                {
                    return res;
                }
                else
                {
                    //calculate the overlap
                    float overlap = std::min(m1_max, m2_max) - std::max(m1_min, m2_min);

                    //check if one mesh is inside the other
                    if((m1_max < m2_max && m1_min > m2_min) || (m1_max > m2_max && m1_min < m2_min))
                    {
                        float mins = std::fabs(m1_min - m2_min);
                        float maxs = std::fabs(m1_max - m2_max);

                        if(mins < maxs) { overlap += mins; }
                        else            { overlap += maxs; }
                    }

                    //update the final overlap and minimal translation vector
                    if(overlap < min_overlap)
                    {
                        min_overlap = overlap;
                        min_trans_vec = t_normal * min_overlap;

                        if(glm::dot(delta, min_trans_vec) > 0)
                        {
                            min_trans_vec *= -1.0f;
                        }
                    }
                }
            }

            //for every triangle in mesh 2
            for(u32 i = 0; i < m2_vertices.size(); i += 3)
            {
                //calculate triangle normal
                glm::vec3 t_normal = glm::normalize(m2->getVertex(i).value().nor);
        
                float projection;

                //save bounds of the projected mesh
                float m1_min;
                float m1_max;

                //project mesh 1 onto the triangle normal
                for(u32 j = 0; j < m1_vertices.size(); j++)
                {
                    projection = glm::dot(m1->getVertex(j).value().pos, t_normal);

                    if(j == 0)
                    {
                        m1_min = m1_max = projection;
                    }
                    else
                    {
                        if(projection < m1_min) { m1_min = projection; }
                        if(projection > m1_max) { m1_max = projection; }
                    }
                }

                //save bounds of the projected mesh
                float m2_min;
                float m2_max;

                //project mesh 2 onto the triangle normal
                for(u32 j = 0; j < m2_vertices.size(); j++)
                {
                    projection = glm::dot(m2->getVertex(j).value().pos, t_normal);

                    if(j == 0)
                    {
                        m2_min = m2_max = projection;
                    }
                    else
                    {
                        if(projection < m2_min) { m2_min = projection; }
                        if(projection > m2_max) { m2_max = projection; }
                    }
                }

                //if there is a gap between meshes -> no collision
                if(!(m1_min <= m2_max && m2_min <= m1_max))
                {
                    return res;
                }
                else
                {
                    //calculate the overlap
                    float overlap = std::min(m1_max, m2_max) - std::max(m1_min, m2_min);

                    //check if one mesh is inside the other
                    if((m1_max < m2_max && m1_min > m2_min) || (m1_max > m2_max && m1_min < m2_min))
                    {
                        float mins = std::fabs(m1_min - m2_min);
                        float maxs = std::fabs(m1_max - m2_max);

                        if(mins < maxs) { overlap += mins; }
                        else            { overlap += maxs; }
                    }

                    //update the final overlap and minimal translation vector
                    if(overlap < min_overlap)
                    {
                        min_overlap = overlap;
                        min_trans_vec = t_normal * min_overlap;

                        if(glm::dot(delta, min_trans_vec) > 0)
                        {
                            min_trans_vec *= -1.0f;
                        }
                    }
                }
            }

            res.occurred     = true;
            res.displacement = min_trans_vec;

            return res;
        }
    
        /**
         * mesh vs mesh
         */
         Data MeshVsMesh(SceneObject* m1, SceneObject* m2)
         {
            Data res;
            
            //extract vertices
            const std::vector<Vertex>& m1_vertices = *m1->vertices();
            const std::vector<Vertex>& m2_vertices = *m2->vertices();
            
            for(u32 i = 0; i < m1_vertices.size(); i += 3)
            {
                for(u32 j = 0; j < m2_vertices.size(); j += 3)
                {
                    Triangle t1 = m1->constructTriangle(i);
                    Triangle t2 = m2->constructTriangle(j);
                    
                    res = TriangleVsTriangle(t1, t2);
                    if(res)
                    {
                        return res;
                    }
                }
            }
             
            return res;
         }

         Data MeshVsMeshSweep(SceneObject* m1, const glm::vec3& velocity, SceneObject* m2)
         {
             Data res;

             //extract vertices
             const std::vector<Vertex>& m1_vertices = *m1->vertices();
             const std::vector<Vertex>& m2_vertices = *m2->vertices();

             for (u32 i = 0; i < m1_vertices.size(); i += 3)
             {
                 for (u32 j = 0; j < m2_vertices.size(); j += 3)
                 {
                     Triangle t1 = m1->constructTriangle(i);
                     Triangle t2 = m2->constructTriangle(j);

                     res = TriangleVsTriangleSweep(t1, velocity, t2);
                     if (res)
                     {
                         return res;
                     }
                 }
             }

             return res;
         }

         Data MeshVsMeshPartial(SceneObject* m1, SceneObject* m2)
         {
             Data res;

             // go through every subspace of the first object
             for (u32 z1 = 0; z1 < DivisionFactor; z1++)
             {
                 for (u32 y1 = 0; y1 < DivisionFactor; y1++)
                 {
                     for (u32 x1 = 0; x1 < DivisionFactor; x1++)
                     {
                         //construct a subspace bounding box
                         Box box1 = m1->getDivisionBox(x1, y1, z1);

                         // go through every subspace of the second object
                         for (u32 z2 = 0; z2 < DivisionFactor; z2++)
                         {
                             for (u32 y2 = 0; y2 < DivisionFactor; y2++)
                             {
                                 for (u32 x2 = 0; x2 < DivisionFactor; x2++)
                                 {
                                     Box box2 = m2->getDivisionBox(x2, y2, z2);

                                     //if them two boxes are overlapping -> potential collision
                                     if (Collision::BoxVsBox(box1, box2))
                                     {
                                         const std::vector<u32> partial_division1 = m1->getDivision(x1, y1, z1);
                                         const std::vector<u32> partial_division2 = m2->getDivision(x2, y2, z2);
                                     
                                         return res;

                                         for (u32 i = 0; i < partial_division1.size(); i++)
                                         {
                                             for (u32 j = 0; j < partial_division2.size(); j++)
                                             {
                                                 u32 index1 = partial_division1[i];
                                                 u32 index2 = partial_division2[j];

                                                 Triangle t1 = m1->constructTriangle(index1 - (index1 % 3));
                                                 Triangle t2 = m2->constructTriangle(index2 - (index2 % 3));

                                                 res = TriangleVsTriangle(t1, t2);
                                                 if (res)
                                                 {
                                                     return res;
                                                 }
                                             }
                                         }
                                     }
                                 }
                             }
                         }
                     }
                 }
             }

             return res;
         }
    };
};