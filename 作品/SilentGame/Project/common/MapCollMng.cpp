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
    //auto checkCross = [](Vector2& vec1, Vector2& vec2)
    //{
    //    return ((vec1.x * vec2.y) - (vec2.x * vec1.y));
    //};
    //Vector2& vecRay = ray.vec;
    //Vector2&& vecLine = line.end - line.p;
    //auto crossRayLine = checkCross(vecRay, vecLine);
    //if (crossRayLine == 0)
    //{
    //    return { false,ray.vec };
    //}
    //Vector2 v = Vector2{ line.p.x - ray.p.x,line.p.y - ray.p.y };
    //auto cross_vRay = checkCross(v, vecRay);
    //auto cross_vLine = checkCross(v, vecLine);

    //float ans1 = cross_vRay / crossRayLine;
    //float ans2 = cross_vLine / crossRayLine;
    //if (ans1 > 0.0f && ans1 <= 1.0f &&
    //    ans2 > 0.0f && ans2 <= 1.0f)
    //{
    //    return { true,vecRay * ans2 };
    //}
    //return { false,ray.vec };

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

  /*  if (valMax == 0.0f)
    {
       
        return { false, ray.vec};
    }*/

    float tmp{ std::abs(cross1) / valMax };

    return { true , ray.p - (ray.p + (ray.vec * tmp)) };
}
