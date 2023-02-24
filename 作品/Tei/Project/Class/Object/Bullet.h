#pragma once
#include "Object.h"
class Bullet :
    public Object
{
public:
    Bullet(const Vector2Flt& pos, const Vector2& size, const Object_ID ,const Vector2Flt& speed,bool reverseXFlag,TeamTag tag);
    ~Bullet();
    virtual void Update(const double& delta, std::weak_ptr<MapData> mapData)override;
    virtual void HitCollision(std::shared_ptr<Object> otehr)override;
private:

protected:
    Vector2Flt speed_;
};

