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
    /// <summary> �R���X�g���N�^ </summary>
    /// <param name="objMng"> Obj�N���X��Manager </param>
    /// <param name="offset"> �I�t�Z�b�g </param>
    /// <param name="drawMng"> �`��Manager </param>
    /// <param name="beforScene"> �O�V�[�� </param>
    /// <param name="afterScene"> ��V�[�� </param>
    ClearTrans(std::shared_ptr<ObjManager> objMng, DrawMng& drawMng, Camera& camera, std::shared_ptr<TmxObj> tmxObj, std::shared_ptr<GmkEvent> gEvent, uniqueScene beforScene, uniqueScene afterScene);
    ~ClearTrans();
private:
    /// <summary> �g�����W�V�����X�V </summary>
    /// <param name="delta"> �f���^�^�C�� </param>
    /// <returns> �g�����W�V��������:true </returns>
    bool UpdataTransition(double delta);

    /// <summary> ��ʃY�[���̍X�V </summary>
    void UpdateZoom(double delta);

    /// <summary> �a���A�j���[�V�����̍X�V </summary>
    void UpdateSlash(double delta);

    /// <summary> ���j�A�j���[�V�����̍X�V </summary>
    void UpdateExplosion(double delta);

    /// <summary> �g�����W�V�����`�� </summary>
    /// <param name="delta"> �f���^�^�C�� </param>
    void DrawOwnScreen(float delta) override;

    /// <summary> ��ʃY�[���̕`�� </summary>
    void DrawZoom(double delta);

    /// <summary> �a���A�j���[�V�����̕`�� </summary>
    void DrawSlash(double delta);

    /// <summary> ���j�A�j���[�V�����̕`�� </summary>
    void DrawExplosion(double delta);

    /// <summary> �X�V�p�̃t�@���N�V���� </summary>
    std::function<void(double)> updateFunc_;

    /// <summary> �`��p�̃t�@���N�V���� </summary>
    std::function<void(double)> drawFunc_;

    /// <summary> �g�����W�V���������t���O </summary>
    bool isEnd_;

    /// <summary> �f���^�^�C�� </summary>
    //double delta_;

    /// <summary> �ő厞�� </summary>
    double limitTime_;

    /// <summary> �`��Manager </summary>
    DrawMng& drawmng_;

    // ��ʃY�[���{��
    double zoomExt_;

    Math::Vector2 zoomPos_;

    Camera& camera_;

    int viewID_;

    std::shared_ptr<TmxObj> tmxObj_;

    std::shared_ptr<GmkEvent> gEvent_;

    bool effectF_;

    Math::Vector2 distance_;
};

