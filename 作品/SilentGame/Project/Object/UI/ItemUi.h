#pragma once
#include "UiBase.h"
#include "../../Input/Controller.h"
#include "../Unit/state/PlayerState.h"

class ItemUi :
    public UiBase
{
public:
    // コンストラクタ 
    ItemUi(const Math::Vector2& pos, const Math::Vector2& UISize);
    ~ItemUi();
private:
    // 更新 
    void Update(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    void UpdateStart(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    void UpdateGame(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    // UI部分描画 
    void DrawUI(const double& delta) override;

    // ゲーム開始時描画
    void DrawStart(const double& delta) override;

    // ゲーム中描画
    void DrawGame(const double& delta) override;

    // UIアイテム描画 
    void DrawItem();

    // ID取得 
    UIID GetUIID(void) override { return UIID::ItemUI; }

    // スタート演出が終わったか
    bool IsStartFin() override { return startFinItem_; }

    // アイテムサイズ 
    Math::Vector2 size_;

    // ゲーム開始時の演出終了フラグ 
    bool startFinItem_;

    Math::Vector2 startBox_;
};

