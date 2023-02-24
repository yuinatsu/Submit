#pragma once
#include <list>
#include <memory>
#include <vector>
#include <functional>
#include "../BaseScene.h"
#include "../../Object/Obj.h"
#include "../../common/Math.h"
class ObjManager;

/// <summary> �g�����W�V�����̎�� </summary>
enum class Transition
{
    Fade,           // �t�F�[�h�C���A�E�g
    Circle,         // �T�[�N����Ƀg�����W�V����
    Clear,          // �N���A�����o����̃t�F�[�h�C���A�E�g
    Dead,           // �Q�[���I�[�o�[�����o����̃t�F�[�h�C���A�E�g
    Max
};

// ��ʑJ�ڗp�N���X
class TransitionScene :
    public BaseScene
{
public:
    /// <summary> �R���X�g���N�^ </summary>
    /// <param name="beforScene"> �ڍs�O�V�[�� </param>
    /// <param name="afterScene"> �ڍs��V�[�� </param>
    TransitionScene(uniqueScene beforScene, uniqueScene afterScene);
    ~TransitionScene();
private:
    /// <summary> ������ </summary>
    /// <returns> ���퓮��:true </returns>
    bool Init(void);

    /// <summary> �X�V </summary>
    /// <param name="delta"> �f���^�^�C�� </param>
    /// <param name="ownScene"> ���݂̃V�[�� </param>
    /// <returns> ���݂̃V�[�� </returns>
    uniqueScene Update(float delta, uniqueScene ownScene) override;

    /// <summary> �`�� </summary>
    void Draw(void);

    /// <summary> �g�����W�V�����X�V </summary>
    /// <param name="delta"> �f���^�^�C�� </param>
    /// <returns> �g�����W�V��������:true </returns>
    virtual bool UpdataTransition(double delta) = 0;
protected:
    /// <summary> �V�[��ID�擾 </summary>
    /// <returns> SceneID </returns>
    Scene GetSceneID(void) override final { return Scene::Transtion; };

    /// <summary> �O�V�[�� </summary>
    uniqueScene beforScene_;

    /// <summary> ��V�[�� </summary>
    uniqueScene afterScene_;

    /// <summary> �V�[���ڍs�t���O�擾 </summary>
    /// <returns> �V�[���ڍs�t���O </returns>
    bool GetFlag(void) override { return SceneFlag_; };

    /// <summary> �J�����T�C�Y </summary>
    Math::Vector2 cameraSize_;

    /// <summary> ��ʃI�t�Z�b�g�T�C�Y </summary>
    Math::Vector2 offsetSize_;

    /// <summary> �I�t�Z�b�g </summary>
    Math::Vector2 offset_;

    /// <summary> Obj�N���X��Manager </summary>
    std::shared_ptr<ObjManager> objMng_;

    /// <summary> �t�F�[�h�A�E�g�����̃J�E���^ </summary>
    double count;
};

