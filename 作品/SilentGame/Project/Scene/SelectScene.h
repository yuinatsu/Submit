#pragma once
#include <vector>
#include "../common/MapInfo.h"
#include "BaseScene.h"

template<class T>
class MenuUICtrl;

class TmxObj;
class ObjManager;
class GmkEvent;
class ResultData;
class UIMng;

enum class SELECT_TYPE
{
    TOne,
    TTwo,
    TThree,
    StageOne,
    StageTwo,
    StageThree,
    StageFour,
    StageFive,
    StageSix,
    StageSeven,
    StageEight,
    StageNine,
    StageTen,
    StageEleven,
    StageTwelve,
    StageThirteen,
    StageFourteen,
    StageFifteen,
    //Title,
    Max,
};

enum class SELECT_TYPE2
{
    StageSelect,
    Other,
    Max,
};

enum class SELECT_TYPE3
{
    Title,
    Setting,
    Max,
};
class SelectScene :
    public BaseScene
{
public:
    SelectScene();
    ~SelectScene();
    void SoundPlay(void) override;
private:
    bool Init(void) override;
    void InitGame(void);
    void InitScreen(void);
    uniqueScene Update(float delta, uniqueScene ownScene) override;
    uniqueScene UpdateSetting(float delta, uniqueScene ownScene);
    uniqueScene UpdateNext(uniqueScene ownScene);         // どのマップに挑戦するか
    void DrawOwnScreen(float delta) override;                     // 自分自身を描画
    // 背景の描画
    void DrawBg();

    // セレクト画面にするかどうか
    void DrawSetting(void);

    /// <summary> 全体のマップを表示する </summary>
    /// <param name="StageID">ステージが何番なのか</param>
    /// <param name="Left">選択カーソル(左)を表示するか基本はtrue</param>
    /// <param name="Right">選択カーソル(右)を表示するか基本はtrue</param>
    void DrawMap(int StageID, bool Left = true, bool Right = true);

    Scene GetSceneID(void) override { return Scene::Select; };    // 自分はセレクトシーン
    bool GetFlag(void) override { return SceneFlag_; };

    /// <summary> セレクト画面で表示するマップに関するデータを読み込む </summary>
    /// <param name=""></param>
    /// <returns> 成功時true失敗時false </returns>
    bool LoadStageData(void);

    void Relese(void);

    // マップのサイズに合わせたDraw
    std::string map_;
    std::shared_ptr<TmxObj> tmxObj_;

    // 作られていないマップが選ばれた場合
    bool noMap_;

    // 仮置きで遅延
    float time_;

    int selecterPic_;
    int selecterLPic_;

    int selectType_;
    int selectType2_;

    // ステージ選択のマップの外枠
    int gameMap_;
 
    float moveMap_;

    // 前のマップ
    float moveMapOld_ = 0.0f;

    // どっちから移動を開始するか
    bool moveMapR_;
    bool moveMapL_;

    // 背景
    int far_buildingsPos_ = 550;
    int buildings_ = 550;
    int skill_foreground_ = 550;
    int fogPosX = 0;

    int ScreenSizeX = 1060;
    int ScreenSizeY = 660;

    float mapPicPosX = (float)(ScreenSizeX / 2);
    float mapPicPosY = (float)(ScreenSizeY / 2 - 100);

    // メニューUIを管理するクラス
    std::unique_ptr< MenuUICtrl<SELECT_TYPE3>> menuUICtrl_;

    // マップ情報
    std::vector<MapInfo> stageData_;
};