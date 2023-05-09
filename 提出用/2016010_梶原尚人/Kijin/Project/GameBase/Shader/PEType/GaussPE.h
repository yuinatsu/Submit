#pragma once
#include <array>
#include "../PEBase.h"

class GaussPE :
    public PEBase
{
public:
    GaussPE(int postPS, Vector2 pos, Vector2 rate);
    ~GaussPE();
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

    PEID GetPEID(void)override { return PEID::Gaussian; }

    // ���_�V�F�[�_
    std::array<int, 2> VS_;
};

