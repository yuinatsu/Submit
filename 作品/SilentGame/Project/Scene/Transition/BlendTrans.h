#pragma once
#include "TransitionScene.h"
class BlendTrans :
    public TransitionScene
{
public:
    /// <summary> �R���X�g���N�^ </summary>
    /// <param name="limitTime"> �g�����W�V�������s���� </param>
    /// <param name="beforScene"> �O�V�[�� </param>
    /// <param name="afterScene"> ��V�[�� </param>
    BlendTrans(double limitTime, uniqueScene beforScene, uniqueScene afterScene);
    ~BlendTrans();
private:
    /// <summary> �g�����W�V�����X�V </summary>
    /// <param name="delta"> �f���^�^�C�� </param>
    /// <returns> �g�����W�V��������:true </returns>
    bool UpdataTransition(double delta);

    /// <summary> �g�����W�V�����`�� </summary>
    /// <param name="delta"> �f���^�^�C�� </param>
    void DrawOwnScreen(float delta) override;

    // <summary> �ő厞�� </summary>
    double limitTime_;

    /// <summary> �g�嗦 </summary>
    float addExt_;
};

