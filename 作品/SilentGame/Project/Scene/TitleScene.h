#pragma once
#include <memory>
#include <list>
#include "BaseScene.h"

// 項目
enum class TSELECT
{
    Start,      // セレクトシーンに遷移
    Setting,    // 設定
    Suspension, // ゲーム終了
    Max,
};

enum class TST
{
    YES,
    NO,
    MAX,
};
template<class T>
class MenuUICtrl;

class TitleScene :
    public BaseScene
{
public:
    TitleScene();
    ~TitleScene();
    void SoundPlay(void) override;
private:
    bool Init(void) override;
    void InitGame(void);
    void InitScreen(void);
    uniqueScene Update(float delta, uniqueScene ownScene) override;

    /// <summary> 通常時のアップデート </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    uniqueScene UpdateGame(float delta, uniqueScene ownScene);
    
    /// <summary> 終了を選択時アップデート </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    uniqueScene UpdateEnd(float delta, uniqueScene ownScene);

    /// <summary> 終了選択し開くまでのアップデート </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    uniqueScene UpdateOpenEnd(float delta, uniqueScene ownScene);

    /// <summary> 終了選択時閉じるまでのアップデート </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    uniqueScene UpdateCloseEnd(float delta, uniqueScene ownScene);

    void DrawLogoAndBg();

    void DrawOwnScreen(float delta) override;                      // 自分自身を描画
    

    /// <summary> 通常時の描画 </summary>
    /// <param name="delta"></param>
    void DrawGame(float delta);

    /// <summary> 終了を選択時の描画 </summary>
    /// <param name="delta"></param>
    void DrawEnd(float delta);

    /// <summary> 終了時を選択し開くまでの描画 </summary>
    /// <param name="delta"></param>
    void DrawOpenEnd(float delta);

    /// <summary> 終了時を選択し閉じるまでの描画 </summary>
    /// <param name="delta"></param>
    void DrawCloseEnd(float delta);

    Scene GetSceneID(void) override { return Scene::Title; };    // 自分はゲームシーン
    bool GetFlag(void) override { return SceneFlag_; };
    void DrawBG(void);
    void DrawKeyUD(void);
    void DrawKeyLR(void);
    void DrawConfirmation(void);

    void Relese(void);

   
    float time_;

    int far_buildingsPos_ = 550;
    int buildings_ = 550;
    int skill_foreground_ = 550;

    int blendGraph_;

    int drawBlendTime_ = 0;

    int blendNozeGraph_;
    int drawBlendNozeTime_ = 10;

    // アップデート処理のメンバ関数ポインタ
    uniqueScene (TitleScene::* updateFunc_)(float, uniqueScene);

    // 描画処理のメンバ関数ポインタ
    void (TitleScene::* drawFunc_)(float);

    // メニューUIをコントロールするクラス
    std::unique_ptr< MenuUICtrl<TSELECT>> menuUICtrl_;
    // もう一つのメニューUI
    std::unique_ptr<MenuUICtrl<TST>> tstUICtrl_;

    // 終了選択時のダイアログ用スクリーンハンドル
    int endWndScreenH_;
};

