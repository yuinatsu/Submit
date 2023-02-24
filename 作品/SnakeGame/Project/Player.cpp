#include <DxLib.h>
#include "Player.h"
#include "_debug/_DebugDispOut.h"
#include "_debug/_DebugConOut.h"

int Player::pCnt = 0;                // 初期値  プレイヤー総数０

// Playerの生成
//---------------
Player::Player()
{
    id_ = pCnt % PLAYER_MAX;                            // プレイヤー総数の余剰で無理やりid_を増えないようにする
    pCnt++;                                             // カウントアップ

    Init(CHAR_ID::MAX);                                 // Initを呼び出し

    memset(keyNow_, 0, sizeof(keyNow_));                // keyNowをセット
    memset(keyOld_, 0, sizeof(keyOld_));                // keyOldをセット
}


// Playerの破棄
//---------------
Player::~Player()
{
    pCnt--;                                             // カウント減少
}


// Playerの更新
//---------------
PL_ST Player::Update(MapCtl& mapCtl, char* keyData, char* keyDataOld)
{
    if (state_ == PL_ST::DEAD)                                                  // もしすでに死んでいたらリターン
    {
        return state_;                                                          // DEADをリターン
    }
    
    GetKeyState(keyData);                                                       // キーデータ取得

    // キャラ移動関係
    //-----------------
    if ((pos_.x % CHIP_SIZE == 0) && (pos_.y % CHIP_SIZE == 0))                 // ブロックの中心を通ってるか判断        座標 % １つのブロックのサイズ = 0 の時、移動している道はブロックの中央になる
    {
        if (mapCtl.isOnBlock(pos_))                                             // isOnBlockがtrueであればstate_の状態を変化させる
        {
            state_ = PL_ST::DEAD;                                               // 死んだ状態にする
            return state_;                                                      // DEADをリターン
        }

        mapCtl.SetBlock(pos_);                                                  // ブロックの配置

        for (int dir = 0; dir < static_cast<int>(DIR::MAX); dir++)
        {
            if (keyNow_[dir])
            {
                dir_ = static_cast<DIR>(dir);                                   // 方向指定
            }
        }
    }

    switch (dir_)                                                               // キャラの移動
    {
    case DIR::LEFT:
        pos_.x -= PLAYER_DEF_SPEED;
        break;

    case DIR::RIGHT:
        pos_.x += PLAYER_DEF_SPEED;
        break;

    case DIR::UP:
        pos_.y -= PLAYER_DEF_SPEED;
        break;

    case DIR::DOWN:
        pos_.y += PLAYER_DEF_SPEED;
        break;
    }

    pAnimCnt_++;                                                                // アニメーションカウントアップ

    return state_;                                                              // 何もなければALIVEをリターン
}


// キーデータ取得
//-----------------
void Player::GetKeyState(char* keyData)
{
    memcpy(keyOld_, keyNow_, sizeof(keyNow_));                          // 最新の情報(keyNow)をkeyOldに格納

    for (int dir = 0; dir < static_cast<int>(DIR::MAX); dir++)          // keyListのデータをkeyNowに格納
    {
        keyNow_[dir] = keyData[keyList_[dir]];
    }
}


// Playerの描画
//---------------
void Player::Draw(void)
{
    DrawGraph(DRAW_OFFSET_X + pos_.x, DRAW_OFFSET_Y + pos_.y - CHIP_SIZE / 3, charImage_.chipImage[static_cast<int>(dir_)][abs(((pAnimCnt_ / 10) % 4) - 2)], true);                       // キャラ描画           // 横移動の際違和感があるので、グラフィックだけずらして内部処理だけ正常にすることで解決させる

    if (id_ == 0)
    {
        DrawTurnGraph(IconPos_.x, 0, charImage_.iconImage_, true);                      // 1Pキャラアイコン
    }
    else
    {
        DrawGraph(IconPos_.x, 0, charImage_.iconImage_, true);                          // 2Pキャラアイコン
    }
}


// Playerの初期化
//-----------------
bool Player::Init(CHAR_ID charID)
{
    if (id_ == 0)                                                   // 1Pの情報
    {
        charID_ = CHAR_ID::REIMU;
        keyList_[static_cast<int>(DIR::DOWN)] = KEY_INPUT_S;        // 下
        keyList_[static_cast<int>(DIR::LEFT)] = KEY_INPUT_A;        // 左
        keyList_[static_cast<int>(DIR::RIGHT)] = KEY_INPUT_D;       // 右
        keyList_[static_cast<int>(DIR::UP)] = KEY_INPUT_W;          // 上

        dir_ = DIR::RIGHT;                                          // 方向

        IconPos_.x = DRAW_OFFSET_X + CHIP_SIZE;                     // アイコンのX座標

        pos_.x = CHIP_SIZE * 4;                                     // プレイヤーのX座標
    }
    else                                                                 // 2Pの情報
    {
        charID_ = CHAR_ID::MARISA;
        keyList_[static_cast<int>(DIR::DOWN)] = KEY_INPUT_DOWN;          // 下
        keyList_[static_cast<int>(DIR::LEFT)] = KEY_INPUT_LEFT;          // 左
        keyList_[static_cast<int>(DIR::RIGHT)] = KEY_INPUT_RIGHT;        // 右
        keyList_[static_cast<int>(DIR::UP)] = KEY_INPUT_UP;              // 上

        dir_ = DIR::LEFT;                                                // 方向

        IconPos_.x = DRAW_OFFSET_X + CHIP_SIZE * 19 + (CHIP_SIZE / 2);   // アイコンのX座標

        pos_.x = CHIP_SIZE * (MAP_CHIP_COUNT_X - 5);                     // プレイヤーのX座標
    }

    state_ = PL_ST::NON;                                // キャラの初期状態の情報

    if (charID != CHAR_ID::MAX)
    {
        charID_ = charID;                               // charID_に受け取った引数を代入
        charImage_.Init(charID_);                       // キャラ画像を読み込み
        state_ = PL_ST::ALIVE;                          // キャラの状態変化
    }

    pos_.y = CHIP_SIZE * 5;                             // プレイヤーのY座標

    speed_ = PLAYER_DEF_SPEED;                          // プレイヤーの移動量

    pAnimCnt_ = 0;                                      // アニメーションカウント

    return true;
}


// Playerの解放
//---------------
bool Player::Relese(void)
{
    charImage_.Release();                   // charImageの解放
    return true;
}
