#pragma once
#include <memory>
#include <list>
#include "BaseScene.h"

// ����
enum class TSELECT
{
    Start,      // �Z���N�g�V�[���ɑJ��
    Setting,    // �ݒ�
    Suspension, // �Q�[���I��
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

    /// <summary> �ʏ펞�̃A�b�v�f�[�g </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    uniqueScene UpdateGame(float delta, uniqueScene ownScene);
    
    /// <summary> �I����I�����A�b�v�f�[�g </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    uniqueScene UpdateEnd(float delta, uniqueScene ownScene);

    /// <summary> �I���I�����J���܂ł̃A�b�v�f�[�g </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    uniqueScene UpdateOpenEnd(float delta, uniqueScene ownScene);

    /// <summary> �I���I��������܂ł̃A�b�v�f�[�g </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    uniqueScene UpdateCloseEnd(float delta, uniqueScene ownScene);

    void DrawLogoAndBg();

    void DrawOwnScreen(float delta) override;                      // �������g��`��
    

    /// <summary> �ʏ펞�̕`�� </summary>
    /// <param name="delta"></param>
    void DrawGame(float delta);

    /// <summary> �I����I�����̕`�� </summary>
    /// <param name="delta"></param>
    void DrawEnd(float delta);

    /// <summary> �I������I�����J���܂ł̕`�� </summary>
    /// <param name="delta"></param>
    void DrawOpenEnd(float delta);

    /// <summary> �I������I��������܂ł̕`�� </summary>
    /// <param name="delta"></param>
    void DrawCloseEnd(float delta);

    Scene GetSceneID(void) override { return Scene::Title; };    // �����̓Q�[���V�[��
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

    // �A�b�v�f�[�g�����̃����o�֐��|�C���^
    uniqueScene (TitleScene::* updateFunc_)(float, uniqueScene);

    // �`�揈���̃����o�֐��|�C���^
    void (TitleScene::* drawFunc_)(float);

    // ���j���[UI���R���g���[������N���X
    std::unique_ptr< MenuUICtrl<TSELECT>> menuUICtrl_;
    // ������̃��j���[UI
    std::unique_ptr<MenuUICtrl<TST>> tstUICtrl_;

    // �I���I�����̃_�C�A���O�p�X�N���[���n���h��
    int endWndScreenH_;
};

