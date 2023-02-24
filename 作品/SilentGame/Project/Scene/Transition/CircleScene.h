#pragma once
#include "TransitionScene.h"
class CircleScene :
    public TransitionScene
{
public:
    /// <summary> �R���X�g���N�^ </summary>
    /// <param name="limitTime"> �ő厞�� </param>
    /// <param name="beforScene"> �O�V�[�� </param>
    /// <param name="afterScene"> ��V�[�� </param>
    CircleScene(double limitTime, uniqueScene beforScene, uniqueScene afterScene);
    ~CircleScene();
private:
    /// <summary> �g�����W�V�����X�V </summary>
    /// <param name="delta"> �f���^�^�C�� </param>
    /// <returns> �g�����W�V��������:true </returns>
    bool UpdataTransition(double delta);

    /// <summary> �g�����W�V�����`�� </summary>
    /// <param name="delta"> �f���^�^�C�� </param>
    void DrawOwnScreen(float delta) override;

    /// <summary></summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="r"></param>
    /// <param name="Color"></param>
    /// <returns></returns>
    int DrawReversalCircle(int x, int y, int r, int Color);

    /// <summary> �ő厞�� </summary>
    double limitTime_;
};

