#pragma once
#include "UiBase.h"

class StaminaUi :
    public UiBase
{
public:
    // �R���X�g���N�^ 
    StaminaUi(const Math::Vector2& pos, const Math::Vector2& gageDiv);
    ~StaminaUi();
private:
    // �X�V 
    void Update(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    void UpdateStart(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    void UpdateGame(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    // �X�^�~�i�X�V 
    void UpdateStamina(const double& delta);

    // �X�^�~�i�l�Z�b�g 
    void SetStaminaValue(const double& value);

    // UI�`�� 
    void DrawUI(const double& delta) override;

    void DrawStart(const double& delta) override;

    void DrawGame(const double& delta) override;

    // UI��ID�擾 
    UIID GetUIID(void) override { return UIID::StaminaUI; }

    bool IsStartFin() override { return startFinSta_; }

    // �_�b�V���t���O 
    bool isDash_;

    // �ړ����t���O 
    bool isMove_;

    // �_�b�V���\�t���O 
    bool canDash_;

    // �Q�[���J�n���̉��o�I���t���O 
    bool startFinSta_;

    // �Q�[�W�T�C�Y
    Math::Vector2 size_;

    // �g�嗦
    float ext_;

    // �Q�[�W�������[�J�E���^
    float memCnt_;
};

