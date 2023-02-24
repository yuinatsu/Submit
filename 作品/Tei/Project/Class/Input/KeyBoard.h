#pragma once
#include "Controller.h"
class KeyBoard :
    public Controller
{
public:
    KeyBoard();
    
    bool Init(void)override final;

    void Update(void)override final;

    const InputType GetInputType(void)override final;
private:
};

