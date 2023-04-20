#include "MapCollMng.h"

#include "Debug.h"

using namespace Math;

std::pair<bool, Math::Vector2> MapCollMng::CheckCollision(Ray ray, Collision col)
{
    bool flag = false;
    Vector2 moveVec{ ray.vec };
    Line points[4] =
    {
        {// è„ï”
            col.first,
            col.first + Vector2{col.second.x,0},
        },
        {// âEï”
            col.first + Vector2{col.second.x,0},
            col.first + col.second,
        },
        {// â∫ï”
            col.first + col.second,
            col.first + Vector2{0,col.second.y},
        },
        {// ç∂ï”
            col.first + Vector2{0,col.second.y},
            col.first,
        }
    };
    for (auto& point : points)
    {
        auto result = CheckRay(ray, point);
        flag |= result.first;
        moveVec = result.first ? result.second : moveVec;
    }
    return {flag,moveVec};
}

std::tuple<bool, Math::Vector2, Math::Vector2> MapCollMng::CheckColl2(Ray& ray, Collision& col)
{
    bool flag = false;
    Vector2 moveVec{ ray.vec };
    Vector2 colVec;
    Line points[4] =
    {
        {// è„ï”
            col.first,
            col.first + Vector2{col.second.x,0},
        },
        {// âEï”
            col.first + Vector2{col.second.x,0},
            col.first + col.second,
        },
        {// â∫ï”
            col.first + col.second,
            col.first + Vector2{0,col.second.y},
        },
        {// ç∂ï”
            col.first + Vector2{0,col.second.y},
            col.first,
        }
    };
    for (auto point : points)
    {
        auto result = CheckRay(ray, point);
        flag |= result.first;
        moveVec = result.first ? result.second : moveVec;
        colVec = (point.end - point.p).Normalized();
    }
    return std::make_tuple(flag, moveVec, colVec);
}


std::pair<bool, Math::Vector2> MapCollMng::CheckRay(Ray ray, Line line)
{
    Math::Vector2 lineVec{ line.end - line.p };

    float cross1{ Math::Cross(ray.vec,line.end - ray.p) };
    float cross2{ Math::Cross(ray.vec, line.p - ray.p) };
    if (cross1 * cross2 > 0.0f)
    {
        return { false, ray.vec };
    }

    cross1 = Math::Cross(lineVec, ray.p - line.p);
    cross2 = Math::Cross(lineVec, (ray.p + ray.vec) - line.p);
    if (cross1 * cross2 > 0.0f)
    {
        return { false, ray.vec };
    }

    auto valMax{ (std::abs(cross1) + std::abs(cross2)) };

    float tmp{ std::abs(cross1) / valMax };

    return { true , ray.p - (ray.p + (ray.vec * tmp)) };
}
