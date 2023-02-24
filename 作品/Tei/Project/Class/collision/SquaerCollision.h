#pragma once
#include "Collision.h"
#include "../common/Vector2.h"

struct Rect
{
    Vector2Flt pos;
    Vector2Flt size;
};

class SquaerCollision :
    public Collision
{
public:
    SquaerCollision(const Vector2Flt& size, const Vector2Flt offset = {});
    ~SquaerCollision() = default;

    COLLISION_TAG CollisionTag(void)override;

    bool isHit(std::weak_ptr<Collision> col)override;
    const Rect GetRect(void);
    const std::vector<Vector2Flt> GetVertex(void);
    void SetSize(const Vector2Flt& size);
private:
    Vector2Flt size_;
};

