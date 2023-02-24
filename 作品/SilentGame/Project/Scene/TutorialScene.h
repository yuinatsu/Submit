#pragma once
#include <memory>
#include <functional>
#include <list>
#include "BaseScene.h"
#include "../Object/ObjManager.h"
#include "../common/Camera/Camera.h"

class UIMng;
class DrawMng;
class GmkEvent;
class TmxObj;
class ResultData;
class ClearScene;
using VecInt = std::vector<int>;

enum class PLAYER_TYPE
{
    playerOne,
    playerTwo,
    playerThree,
    Max,
};


class TutorialScene :
    public BaseScene
{
public:
    TutorialScene();
    ~TutorialScene();
    void SoundPlay(void) override;
protected:
    bool Init(void) override;
    /// <summary> �X�N���[����`�����̏����� </summary>
/// <param name=""></param>
/// <returns></returns>
    virtual bool InitScreen(void);

    /// <summary> �Q�[���ɂ�����鏉���� </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual bool InitGame(void);

    void UpdateSwitching(float count, bool moveFlag = false, bool hitFlag = false);

    // �I�u�W�F�N�g���Ǘ�����N���X
    std::shared_ptr<ObjManager> objMng_;

    // UI�Ǘ��N���X
    std::shared_ptr<UIMng> uiMng_;

    BoxItem boxItem_;

    // �J�����N���X
    Camera camera_;

    // �M�~�b�N�C�x���g�Ɋւ���N���X
    std::shared_ptr<GmkEvent> gEvent_;
    // �`�揇�Ԃ��Ǘ����`�悷��N���X
    std::unique_ptr< DrawMng> drawMng_;

    // �}�b�v�����[�h���邽��
    std::string map_;
    std::shared_ptr<TmxObj> tmxObj_;

    // �}�b�v��̕`��p�X�N���[��
    int viewID_;

    // �}�b�v�̏��̃X�N���[��
    int mapFloor_;

    bool moveFlag_;
    int TutorialType_;
    float count_;
    VecInt paddata_;
    VecInt keydata_;
    // �Q�[�����J�n���ꂽ���ǂ���
    bool startedFlag_;

    int blendPic_;

    // �Q�[���̊T�v�̐������I�������
    bool sumFlag_;
    // �����o�������̏����p�ϐ�
    bool updownFalg_;
    float speed_;

    float nowRate_;
    Math::Vector2I ViewSize_;
    bool hitFlag_;

    bool isFirst_;
    friend ClearScene;	 // ���U���g�V�[���ɓn���Q�[���̌��ʂ̃f�[�^
    std::shared_ptr<ResultData> resultData_;

private:

    uniqueScene Update(float delta, uniqueScene ownScene) override;
    void DrawOwnScreen(float delta) override;                   // �������g��`��
    Scene GetSceneID(void) override { return Scene::Tutorial; };    // �����̓Q�[���V�[��
    bool GetFlag(void) override { return SceneFlag_; };

    /// <summary> �J�n���̍X�V���� </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    uniqueScene UpdateStart(float delta, uniqueScene ownScene);

    /// <summary> �Q�[�����̍X�V���� </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    uniqueScene GameUpdate(float delta, uniqueScene ownScene);

    uniqueScene UpdateGameEnd(float delta, uniqueScene ownScene);

    virtual bool UpdateTutorial(float delta) = 0;
    virtual void DrawTutorial(float delta) = 0;
    virtual void DrawSummary(float delta) = 0;
    virtual void DrawDescription(float delta) = 0;

    void DrawGame(float delta);

    void DrawGameEnd(float delta);

    void DrawMap(void);

    /// <summary> ���[�h�������̏��� </summary>
    /// <param name=""></param>
    void Loaded(void) final;

    // �A�b�v�f�[�g�pfunction
    std::function<uniqueScene(float, uniqueScene)> updateFunc_;

    std::function<void(float)> drawFunc_;

    // ��ʃY�[���{��
    double zoomExt_;

    Math::Vector2 zoomPos_;

    // fog��A���[�g�̐F�̃G�t�F�N�g
    struct ScreenEffect
    {
        Math::Vector2 pos;      // fog�𔖂����钆�S���W
        float radius;           // fog�𔖂�����͈͂̔��a
        float redValue;         // �A���[�g���̐ԐF�̊���(1�`0�̒l)
    };

    // �萔�o�b�t�@�̃n���h��
    int cbuffH_;

    // �萔�o�b�t�@���������p�|�C���^�[
    ScreenEffect* buff_;

    // �s�N�Z���V�F�[�_�̃n���h��
    int psH_;
};

