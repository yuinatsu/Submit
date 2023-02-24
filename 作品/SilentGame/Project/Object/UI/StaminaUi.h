#pragma once
#include "UiBase.h"

class StaminaUi :
    public UiBase
{
public:
    /// <summary> コンストラクタ </summary>
    /// <param name="pos"> UI座標 </param>
    /// <param name="UISize"> UIサイズ </param>
    StaminaUi(const Math::Vector2& pos, const Math::Vector2& gageDiv);
    ~StaminaUi();
private:
    /// <summary> 更新 </summary>
    /// <param name="delta"> デルタタイム </param>
    /// <param name="objMng"> ObjクラスのManager </param>
    void Update(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    void UpdateStart(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    void UpdateGame(const double& delta, const std::shared_ptr<ObjManager> objMng) override;

    /// <summary> スタミナ更新 </summary>
    /// <param name="delta"> デルタタイム </param>
    void UpdateStamina(const double& delta);

    /// <summary> スタミナ値セット </summary>
    /// <param name="value"> 加減算値 </param>
    void SetStaminaValue(const double& value);

    /// <summary> UI描画 </summary>
    /// <param name="delta"> デルタタイム </param>
    void DrawUI(const double& delta) override;

    void DrawStart(const double& delta) override;

    void DrawGame(const double& delta) override;

    /// <summary> UIのID取得 </summary>
    /// <returns> UIID </returns>
    UIID GetUIID(void) override { return UIID::StaminaUI; }

    bool IsStartFin() override { return startFinSta_; }

    /// <summary> ダッシュフラグ </summary>
    bool isDash_;

    /// <summary> 移動中フラグ </summary>
    bool isMove_;

    /// <summary> ダッシュ可能フラグ </summary>
    bool canDash_;

    /// <summary> ゲーム開始時の演出終了フラグ </summary>
    bool startFinSta_;

    Math::Vector2 size_;

    float ext_;

    float memCnt_;
};

