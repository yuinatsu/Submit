#pragma once
// アニメーション名
enum class AnimIndex {
    Idle,                       // 立ち
    StartForwardMove,           // 前方移動開始
    ForwardMove,                // 前方移動中
    EndForwardMove,             // 前方移動終了
    StartLeftMove,              // 左旋回開始
    LeftMove,                   // 左旋回中
    EndLeftMove,                // 左旋回終了
    StartRightMove,             // 右旋回開始
    RightMove,                  // 右旋回中
    EndRightMove,               // 右旋回終了
    Attack1,                    // 攻撃1コンボ目
    Attack2,                    // 攻撃2コンボ目
    Attack3,                    // 攻撃3コンボ目
    AttackAll,                  // 攻撃全体
    Max,
};
