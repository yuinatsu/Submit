#pragma once
#include "Keyboard.h"
class EnemyInput :
    public Keyboard
{
public:
    EnemyInput();
    ~EnemyInput();
private:
    bool Update(Obj& obj, Obj& other) override;
};

