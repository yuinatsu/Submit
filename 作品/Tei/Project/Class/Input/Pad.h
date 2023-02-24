#pragma once
#include "Controller.h"
class Pad :
    public Controller
{
public:
    Pad();

    void Update(void)override final;

    bool Init(void)override final;

    const InputType GetInputType(void)override final;
private:
};

