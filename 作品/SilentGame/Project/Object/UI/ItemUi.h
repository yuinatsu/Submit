#pragma once
#include "UiBase.h"
#include "../../Input/Controller.h"
#include "../Unit/state/PlayerState.h"

class ItemUi :
    public UiBase
{
public:
    /// <summary> コンストラクタ </summary>
    /// <param name="pos"> UI座標 </param>
    /// <param name="UISize"> UIサイズ </param>
    ItemUi(const Math::Vector2& pos, const Math::Vector2& UISize);
    ~ItemUi();
private:
    /// <summary> 更新 </summary>
    /// <param name="delta"> デルタタイム </param>
    /// <param name="objMng"> ObjクラスのManager </param>
    void Update(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    void UpdateStart(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    void UpdateGame(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    /// <summary> UI部分描画 </summary>
    /// <param name="delta"> デルタタイム </param>
    void DrawUI(const double& delta) override;

    void DrawStart(const double& delta) override;

    void DrawGame(const double& delta) override;

    /// <summary> UIアイテム描画 </summary>
    /// <param name="pos"> 描画座標 </param>
    void DrawItem();

    /// <summary> ID取得 </summary>
    /// <returns> ObjID::ItemUI </returns>
    UIID GetUIID(void) override { return UIID::ItemUI; }

    bool IsStartFin() override { return startFinItem_; }

    /// <summary> アイテムサイズ </summary>
    Math::Vector2 size_;

    /// <summary> ゲーム開始時の演出終了フラグ </summary>
    bool startFinItem_;

    Math::Vector2 startBox_;
};

