#pragma once

namespace Engine3D
{
    struct Box
    {
        Box() {}
        Box(const glm::vec3& pos, const glm::vec3& dims) : pos(pos), dims(dims) {}

        glm::vec3 pos  { 0 };
        glm::vec3 dims { 0 };
    };

    struct Ball
    {
        Ball() {}
        Ball(const glm::vec3& pos, float radius) : pos(pos), radius(radius) {}

        glm::vec3 pos    { 0 };
        float     radius { 0 };
    };

    struct Ellipsoid
    {
        Ellipsoid() {}
        Ellipsoid(const glm::vec3& pos, const glm::vec3& dims) : pos(pos), dims(dims) {}

        glm::vec3 pos  { 0 };
        glm::vec3 dims { 0 };
    };

    struct Cylinder
    {
        Cylinder() {}
        Cylinder(const glm::vec3& pos, const glm::vec2& radius) : pos(pos), dims(dims) {}

        glm::vec3 pos  { 0 };
        glm::vec2 dims { 0 };
    };

    struct Ray
    {
        Ray() {}
        Ray(const glm::vec3& pos, const glm::vec3& dir) : pos(pos), dir(dir) {}

        glm::vec3 pos { 0 };
        glm::vec3 dir { 1, 0, 0 };
    };

    struct Plane
    {
        Plane() {}
        Plane(const glm::vec3& pos, const glm::vec3& nor) : pos(pos), nor(nor) {}

        glm::vec3 pos { 0 };
        glm::vec3 nor { 0, 1, 0 };
    };

    struct Line
    {
        Line() {}
        Line(const glm::vec3& p1, const glm::vec3& p2) : p1(p1), p2(p2) {}

        glm::vec3 p1 { 0 };
        glm::vec3 p2 { 0 };
    };

    struct Triangle
    {
        Triangle() {}
        Triangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) : p1(p1), p2(p2), p3(p3) {}

        glm::vec3 p1 { 0 };
        glm::vec3 p2 { 0 };
        glm::vec3 p3 { 0 };
    };

    struct Bean
    {
        Bean() {}
        Bean(const glm::vec3& pos, const glm::vec2& dims) : pos(pos), dims(dims) {}

        glm::vec3 pos  { 0 };
        glm::vec2 dims { 0 };
    };
};