#pragma once
#include <array>
#include "../PEBase.h"
class DofPE :
    public PEBase
{
public:
    DofPE(int postPS, Vector2 pos, Vector2 rate);
    ~DofPE();
private:
    void Update(float delta) override;
    int Draw(int beforeScr, int afterScr, int depth, int skyScr, int redScr)override;

    /// <summary>
    /// ���u���[�̕`��
    /// </summary>
    /// <param name="beforeScr">���̉摜</param>
    void DrawXblur(int beforeScr);

    /// <summary>
    /// �c�u���[�̕`��
    /// </summary>
    /// <param name="beforeScr">���u���[�̉摜</param>
    void DrawYblur();

    PEID GetPEID(void)override { return PEID::Dof; }

    // ���̃X�N���[���摜�ɕϊ�
    int scr1_;
    int scr2_;

    // ���_�V�F�[�_
    std::array<int, 2> VS_;
    int ps_;
};

