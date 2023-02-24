#pragma once
#include <memory>
#include <functional>
#include <list>
#include "BaseScene.h"
#include "../common/Camera/Camera.h"
#include "../common/ShaderID.h"

class ObjManager;
class UIMng;
class DrawMng;
class GmkEvent;
class TmxObj;
class ResultData;
class ClearScene;

template<class T>
class ConstantBuffer;


class GameScene :
    public BaseScene
{
public:
    GameScene(int mapNum, bool isSmallMap);
    ~GameScene();
    void SoundPlay(void) override;

    const bool IsSmallMap(void) const
    {
        return isSmallMap_;
    }

private:
    bool Init(void) override;

    /// <summary> �X�N���[����`�����̏����� </summary>
    /// <param name=""></param>
    /// <returns></returns>
    bool InitScreen(void);

    /// <summary> �Q�[���ɂ�����鏉���� </summary>
    /// <param name=""></param>
    /// <returns></returns>
    bool InitGame(void);

    uniqueScene Update(float delta, uniqueScene ownScene) override;
    void DrawOwnScreen(float delta) override;                   // �������g��`��
    Scene GetSceneID(void) override { return Scene::Game; };    // �����̓Q�[���V�[��
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

    void DrawGame(float delta);

    void DrawGameEnd(float delta);

    void DrawMap(void);

    /// <summary> ���[�h�������̏��� </summary>
    /// <param name=""></param>
    void Loaded(void) final;

    void Relese(void);

    // �I�u�W�F�N�g���Ǘ�����N���X
    std::shared_ptr<ObjManager> objMng_;

    // UI�Ǘ��N���X
    std::shared_ptr<UIMng> uiMng_;

    // �J�����N���X
    Camera camera_;

    // �M�~�b�N�C�x���g�Ɋւ���N���X
    std::shared_ptr<GmkEvent> gEvent_;
    // �`�揇�Ԃ��Ǘ����`�悷��N���X
    std::unique_ptr<DrawMng> drawMng_;

    // �}�b�v�����[�h���邽��
    std::string map_;
    std::shared_ptr<TmxObj> tmxObj_;

    // �}�b�v��̕`��p�X�N���[��
    int viewID_;

    // �}�b�v�̏��̃X�N���[��
    int mapFloor_;

    // �A�b�v�f�[�g�pfunction
    std::function<uniqueScene(float, uniqueScene)> updateFunc_;

    std::function<void(float)> drawFunc_;

    // �Q�[�����J�n���ꂽ���ǂ���
    bool startedFlag_;

    int blendPic_;

    friend ClearScene;	 
    
    // ���U���g�V�[���ɓn���Q�[���̌��ʂ̃f�[�^
    std::shared_ptr<ResultData> resultData_;

    // ��ʃY�[���{��
    double zoomExt_;

    Math::Vector2 zoomPos_;

    float AlarmTime_;
    bool loopFlag_;

    // �}�b�v����������?
    bool isSmallMap_;

    // fog��A���[�g�̐F�̃G�t�F�N�g
    struct ScreenEffect
    {
        Math::Vector2 pos;      // fog�𔖂����钆�S���W
        float radius;           // fog�𔖂�����͈͂̔��a
        float redValue;         // �A���[�g���̐ԐF�̊���(1�`0�̒l)
    };

    ScreenEffect screenEffect_;

    std::unique_ptr<ConstantBuffer<ScreenEffect>> cbuff_;
  
    // �s�N�Z���V�F�[�_��ID
    ShaderID psID_;

    Math::Vector2 viewPos_;
};

