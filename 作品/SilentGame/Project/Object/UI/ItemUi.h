#pragma once
#include "UiBase.h"
#include "../../Input/Controller.h"
#include "../Unit/state/PlayerState.h"

class ItemUi :
    public UiBase
{
public:
    // �R���X�g���N�^ 
    ItemUi(const Math::Vector2& pos, const Math::Vector2& UISize);
    ~ItemUi();
private:
    // �X�V 
    void Update(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    void UpdateStart(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    void UpdateGame(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    // UI�����`�� 
    void DrawUI(const double& delta) override;

    // �Q�[���J�n���`��
    void DrawStart(const double& delta) override;

    // �Q�[�����`��
    void DrawGame(const double& delta) override;

    // UI�A�C�e���`�� 
    void DrawItem();

    // ID�擾 
    UIID GetUIID(void) override { return UIID::ItemUI; }

    // �X�^�[�g���o���I�������
    bool IsStartFin() override { return startFinItem_; }

    // �A�C�e���T�C�Y 
    Math::Vector2 size_;

    // �Q�[���J�n���̉��o�I���t���O 
    bool startFinItem_;

    Math::Vector2 startBox_;
};

