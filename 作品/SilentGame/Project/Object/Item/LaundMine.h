#pragma once
#include "../Obj.h"
class ObjManager;


using EnemyList = std::list<std::shared_ptr<Obj>>;

class LaundMine :
    public Obj
{
public:
    LaundMine(const Math::Vector2& pos);
    ~LaundMine();

    void Init(void);
    void Update(float delta) override;
    void Draw(const Math::Vector2& drawoffset, DrawMng& drawMng) override;
private:
 
    ObjID GetObjID(void) override { return ObjID::LandMine; };
};

