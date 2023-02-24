#pragma once
#include "UiBase.h"

class StaminaUi :
    public UiBase
{
public:
    /// <summary> �R���X�g���N�^ </summary>
    /// <param name="pos"> UI���W </param>
    /// <param name="UISize"> UI�T�C�Y </param>
    StaminaUi(const Math::Vector2& pos, const Math::Vector2& gageDiv);
    ~StaminaUi();
private:
    /// <summary> �X�V </summary>
    /// <param name="delta"> �f���^�^�C�� </param>
    /// <param name="objMng"> Obj�N���X��Manager </param>
    void Update(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    void UpdateStart(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    void UpdateGame(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    /// <summary> �X�^�~�i�X�V </summary>
    /// <param name="delta"> �f���^�^�C�� </param>
    void UpdateStamina(const double& delta);

    /// <summary> �X�^�~�i�l�Z�b�g </summary>
    /// <param name="value"> �����Z�l </param>
    void SetStaminaValue(const double& value);

    /// <summary> UI�`�� </summary>
    /// <param name="delta"> �f���^�^�C�� </param>
    void DrawUI(const double& delta) override;

    void DrawStart(const double& delta) override;

    void DrawGame(const double& delta) override;

    /// <summary> UI��ID�擾 </summary>
    /// <returns> UIID </returns>
    UIID GetUIID(void) override { return UIID::StaminaUI; }

    bool IsStartFin() override { return startFinSta_; }

    /// <summary> �_�b�V���t���O </summary>
    bool isDash_;

    /// <summary> �ړ����t���O </summary>
    bool isMove_;

    /// <summary> �_�b�V���\�t���O </summary>
    bool canDash_;

    /// <summary> �Q�[���J�n���̉��o�I���t���O </summary>
    bool startFinSta_;

    Math::Vector2 size_;

    float ext_;

    float memCnt_;
};

