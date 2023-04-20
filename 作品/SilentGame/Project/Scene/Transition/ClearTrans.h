#pragma once
#include "TransitionScene.h"
#include "../../common/DrawMng.h"
#include "../../common/Camera/Camera.h"
class TmxObj;
class GmkEvent;

class ClearTrans :
    public TransitionScene
{
public:
    // �R���X�g���N�^ 
    ClearTrans(std::shared_ptr<ObjManager> objMng, DrawMng& drawMng, Camera& camera, std::shared_ptr<TmxObj> tmxObj, std::shared_ptr<GmkEvent> gEvent, uniqueScene beforScene, uniqueScene afterScene);
    ~ClearTrans();
private:
    // �g�����W�V�����X�V 
    bool UpdataTransition(double delta);

    // ��ʃY�[���̍X�V 
    void UpdateZoom(double delta);

    // �a���A�j���[�V�����̍X�V 
    void UpdateSlash(double delta);

    // ���j�A�j���[�V�����̍X�V 
    void UpdateExplosion(double delta);

    // �g�����W�V�����`�� 
    void DrawOwnScreen(float delta) override;

    // ��ʃY�[���̕`�� 
    void DrawZoom(double delta);

    // �a���A�j���[�V�����̕`�� 
    void DrawSlash(double delta);

    // ���j�A�j���[�V�����̕`�� 
    void DrawExplosion(double delta);

    // �X�V�p�̃t�@���N�V���� 
    std::function<void(double)> updateFunc_;

    // �`��p�̃t�@���N�V���� 
    std::function<void(double)> drawFunc_;

    // �g�����W�V���������t���O 
    bool isEnd_;

    // �ő厞�� 
    double limitTime_;

    // �`��Manager 
    DrawMng& drawmng_;

    // ��ʃY�[���{��
    double zoomExt_;

    // �Y�[�����W
    Math::Vector2 zoomPos_;

    // �J�������
    Camera& camera_;

    // �Q�[���V�[���̍Ō�̃X�N���[�����
    int viewID_;

    // �X�e�[�W���
    std::shared_ptr<TmxObj> tmxObj_;

    // �M�~�b�N�C�x���g
    std::shared_ptr<GmkEvent> gEvent_;

    // �G�t�F�N�g�t���O
    bool effectF_;

    // �Y�[������
    Math::Vector2 distance_;
};

