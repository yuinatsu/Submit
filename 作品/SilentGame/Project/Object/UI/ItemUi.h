#pragma once
#include "UiBase.h"
#include "../../Input/Controller.h"
#include "../Unit/state/PlayerState.h"

class ItemUi :
    public UiBase
{
public:
    /// <summary> �R���X�g���N�^ </summary>
    /// <param name="pos"> UI���W </param>
    /// <param name="UISize"> UI�T�C�Y </param>
    ItemUi(const Math::Vector2& pos, const Math::Vector2& UISize);
    ~ItemUi();
private:
    /// <summary> �X�V </summary>
    /// <param name="delta"> �f���^�^�C�� </param>
    /// <param name="objMng"> Obj�N���X��Manager </param>
    void Update(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    void UpdateStart(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    void UpdateGame(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    /// <summary> UI�����`�� </summary>
    /// <param name="delta"> �f���^�^�C�� </param>
    void DrawUI(const double& delta) override;

    void DrawStart(const double& delta) override;

    void DrawGame(const double& delta) override;

    /// <summary> UI�A�C�e���`�� </summary>
    /// <param name="pos"> �`����W </param>
    void DrawItem();

    /// <summary> ID�擾 </summary>
    /// <returns> ObjID::ItemUI </returns>
    UIID GetUIID(void) override { return UIID::ItemUI; }

    bool IsStartFin() override { return startFinItem_; }

    /// <summary> �A�C�e���T�C�Y </summary>
    Math::Vector2 size_;

    /// <summary> �Q�[���J�n���̉��o�I���t���O </summary>
    bool startFinItem_;

    Math::Vector2 startBox_;
};

