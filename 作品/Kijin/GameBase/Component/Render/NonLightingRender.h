#pragma once
#include "ModelRender.h"

// ���C�e�B���O�Ƃ����Ȃ��`�������N���X(�X�J�C�h�[���Ƃ�)
class NonLightingRender :
    public ModelRender
{
public:
private:
    void Draw(int shadowMap = -1, int buff = -1) final;
    void SetUpDepthTex(int ps = -1,int buff = -1) final;
};

