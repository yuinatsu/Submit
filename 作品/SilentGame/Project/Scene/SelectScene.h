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
    uniqueScene UpdateNext(uniqueScene ownScene);         // �ǂ̃}�b�v�ɒ��킷�邩
    void DrawOwnScreen(float delta) override;                     // �������g��`��
    // �w�i�̕`��
    void DrawBg();

    // �Z���N�g��ʂɂ��邩�ǂ���
    void DrawSetting(void);

    /// <summary> �S�̂̃}�b�v��\������ </summary>
    /// <param name="StageID">�X�e�[�W�����ԂȂ̂�</param>
    /// <param name="Left">�I���J�[�\��(��)��\�����邩��{��true</param>
    /// <param name="Right">�I���J�[�\��(�E)��\�����邩��{��true</param>
    void DrawMap(int StageID, bool Left = true, bool Right = true);

    Scene GetSceneID(void) override { return Scene::Select; };    // �����̓Z���N�g�V�[��
    bool GetFlag(void) override { return SceneFlag_; };

    /// <summary> �Z���N�g��ʂŕ\������}�b�v�Ɋւ���f�[�^��ǂݍ��� </summary>
    /// <param name=""></param>
    /// <returns> ������true���s��false </returns>
    bool LoadStageData(void);

    void Relese(void);

    // �}�b�v�̃T�C�Y�ɍ��킹��Draw
    std::string map_;
    std::shared_ptr<TmxObj> tmxObj_;

    // ����Ă��Ȃ��}�b�v���I�΂ꂽ�ꍇ
    bool noMap_;

    // ���u���Œx��
    float time_;

    int selecterPic_;
    int selecterLPic_;

    int selectType_;
    int selectType2_;

    // �X�e�[�W�I���̃}�b�v�̊O�g
    int gameMap_;
 
    float moveMap_;

    // �O�̃}�b�v
    float moveMapOld_ = 0.0f;

    // �ǂ�������ړ����J�n���邩
    bool moveMapR_;
    bool moveMapL_;

    // �w�i
    int far_buildingsPos_ = 550;
    int buildings_ = 550;
    int skill_foreground_ = 550;
    int fogPosX = 0;

    int ScreenSizeX = 1060;
    int ScreenSizeY = 660;

    float mapPicPosX = (float)(ScreenSizeX / 2);
    float mapPicPosY = (float)(ScreenSizeY / 2 - 100);

    // ���j���[UI���Ǘ�����N���X
    std::unique_ptr< MenuUICtrl<SELECT_TYPE3>> menuUICtrl_;

    // �}�b�v���
    std::vector<MapInfo> stageData_;
};