#pragma once
#include "TransitionScene.h"
#include "../../common/DrawMng.h"
#include "../../common/Camera/Camera.h"
class DeadTrans :
    public TransitionScene
{
public:
    // �R���X�g���N�^ 
    DeadTrans(std::shared_ptr<ObjManager> objMng, Math::Vector2 offset, DrawMng& drawMng, Camera& camera, uniqueScene beforScene, uniqueScene afterScene);
    ~DeadTrans();
private:
    // �g�����W�V�����X�V 
    bool UpdataTransition(double delta);

    // �g�����W�V�����`�� 
    void DrawOwnScreen(float delta) override;

    // �ő厞�� 
    double limitTime_;

    // �`��Manager 
    DrawMng& drawmng_;

    // �J�������
    Camera& camera_;

    // �Q�[���V�[���̍Ō�̃X�N���[�����
    int viewID_;
};

