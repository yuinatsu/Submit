#pragma once
#include "UiBase.h"

class StaminaUi :
    public UiBase
{
public:
    // コンストラクタ 
    StaminaUi(const Math::Vector2& pos, const Math::Vector2& gageDiv);
    ~StaminaUi();
private:
    // 更新 
    void Update(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    void UpdateStart(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    void UpdateGame(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    // スタミナ更新 
    void UpdateStamina(const double& delta);

    // スタミナ値セット 
    void SetStaminaValue(const double& value);

    // UI描画 
    void DrawUI(const double& delta) override;

    void DrawStart(const double& delta) override;

    void DrawGame(const double& delta) override;

    // UIのID取得 
    UIID GetUIID(void) override { return UIID::StaminaUI; }

    bool IsStartFin() override { return startFinSta_; }

    // ダッシュフラグ 
    bool isDash_;

    // 移動中フラグ 
    bool isMove_;

    // ダッシュ可能フラグ 
    bool canDash_;

    // ゲーム開始時の演出終了フラグ 
    bool startFinSta_;

    // ゲージサイズ
    Math::Vector2 size_;

    // 拡大率
    float ext_;

    // ゲージメモリーカウンタ
    float memCnt_;
};

