#pragma once
#include "ModelRender.h"
class NonLightingRender :
    public ModelRender
{
public:
private:
    void Draw(void) final;
    void Draw(int shadowMap, int buff) final;
    void SetUpShadowMap(void) final;
};

