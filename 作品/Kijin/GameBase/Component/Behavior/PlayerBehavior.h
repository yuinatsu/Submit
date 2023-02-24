#pragma once
#include <map>
#include <string>
#include <vector>
#include "../../Object/ObjectManager.h"
#include "../Transform/Transform.h"
#include "../Collider/CharactorCollider.h"
#include "../../Common/Input/Controller.h"
#include "Behavior.h"
#include "../Render/ModelRender.h"
#include "../Animator/Animator.h"
#include "../../UI/UiID.h"
#include "../../Common/SoundPross.h"
#include "../Animator/PlayerAnimIndex.h"

class PlayerBehavior :
    public Behavior
{
public:
    PlayerBehavior(CntType cnt);

    ComponentID GetID(void) const override
    {
        return id_;
    }
    static constexpr ComponentID id_{ ComponentID::PlayerBehavior };

    // スキルゲージ上昇
    void RiseSkillValue(void);
    // ゲージ情報取得
    std::pair<float, std::pair<float, float>> GetGaugeValue(UiID id) const;
    // コンボ数取得
    int GetComboNum(void) { return combo_; }
    // コンボ数加算
    void AddCombo(void) { combo_++; }
private:
    // 初期化
    bool Init(CntType cnt);
    void Begin(ObjectManager& objectManager) final;
    // 更新
    void Update(BaseScene& scene,ObjectManager& objectManager, float delta, Controller& controller) final;
    // 移動制御
    float ProcessMove(BaseScene& scene,Controller& controller, float speed);
    // ダッシュ制御
    float ProcessDash(Controller& controller,const float& delta);
    // 回転制御
    void ProcessRotate(BaseScene& scene,Controller& controller, float rot);
    // ジャンプ制御
    void ProcessJump(Controller& controller);
    // 攻撃制御
    void ProcessAttack(Controller& controller, ObjectManager& objectManager);
    // スタミナ制御
    void ProcessStamina(Controller& controller);
    // クールタイム計測
    void CoolTimer(float& delta);
    // スキル制御
    void ProcessSkill(Controller& controller);
    // アニメーション制御
    void Animation(Controller& controller);
    // 音関係制御
    void Sound(Controller& controller);
    // 衝突処理
    void OnHit(Collider& col, ObjectManager& objectManager);

    // 回避継続時間
    static constexpr float DodgeLimit = 0.2f;
    // スタミナ切れクールタイム
    static constexpr float CoolTime = 3.0f;
    // 徐々に回転する量
    static constexpr float SpeedRotRad = 5.0f * (DX_PI_F / 180.0f);
    // ゲージ上昇定数
    static constexpr float RiseValue = 0.25f;
    // 攻撃アニメーション最大番号
    static constexpr int AtkAnimMax = 3;

    // 移動量
    struct moveDis {
        float walk = 10.0f;          // 歩き
        float dash = 30.0f;         // ダッシュ
        float dodge = 75.0f;        // 回避
    };
    // 方向角度
    struct dir {
        float left = 90.0f;         // 左
        float right = 270.0f;       // 右
        float up = 180.0f;          // 上
        float down = 0.0f;          // 下
    };
    // ジャンプ
    struct jump {
        float initVel = 10.0f;      // 初速度
        float vel = 0.0f;           // y方向の速度
        float acc = 1.0f;           // 重力加速度
    };
    // アニメーション再生時間
    struct animTime {
        float idle = 0.1f;          // 立ち
        float move = 0.1f;          // 移動
        float attack1 = 0.3f;       // 1撃目
        float attack2 = 0.1f;       // 2撃目
        float attack3 = 0.3f;       // 3撃目
    };

    // アニメーション
    ComponentHandle<Animator> animation_;
    // トランスフォーム
    ComponentHandle<Transform> transform_;
    // コリジョン
    ComponentHandle<CharactorCollider> collider_;

    // 移動量
    moveDis movedis_;
    // ダッシュし始めから経過時間
    float dodgeTimer_;
    // 回避中か
    bool isDodge_;
    // ダッシュ禁止時間
    float noDashTime_;
    // スタミナ切れになったか
    bool isStaminaLoss_;
    // 方向角度
    dir dir_;
    // 回転情報
    Vector3 angles_;
    // ジャンプ
    jump jump_;

    // 攻撃中か
    bool isAttack_;
    // 攻撃アニメーション番号
    int atkAnim_;
    // コンボ数
    int combo_;
    // スキル使用可能か
    bool useSkill_;

    // アニメーション情報
    std::vector<std::pair<AnimIndex, float>> anim_;
    // アニメーションごとの再生時間
    animTime animtime_;

    // ゲージ値情報
    std::map<UiID, std::pair<float, std::pair<float, float>>> gauge_;

    // サウンド
    std::vector< std::pair<SoundProcess::SOUNDNAME_SE, bool>> sound_;
};

