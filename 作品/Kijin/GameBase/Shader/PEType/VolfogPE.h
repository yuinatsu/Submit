#pragma once
#include <array>
#include "../PEBase.h"
class VolfogPE :
    public PEBase
{
public:
    VolfogPE(int postPS, Vector2 pos, Vector2 rate);
    ~VolfogPE();
    // �k��������{��
    static constexpr float REDUCTION = 2.00f;
private:
    void Update(float delta) override;
    int Draw(int beforeScr, int afterScr, int depth, int skyScr, int redScr) override;

    PEID GetPEID(void)override { return PEID::VolFog; }
    // �X�e�[�W�݂̂�`��
    int stageScr_;
    // �k���o�b�t�@
    int shrinkScr_;
    // �t�H�O���������X�N���[��
    int fogScr1_;
    int fogScr2_;
    // �t�H�O�݂̂̃V�F�[�_
    int ps_;
};

