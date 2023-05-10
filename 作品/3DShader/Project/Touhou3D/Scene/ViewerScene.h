#pragma once
#include <list>
#include <memory>
#include <functional>
#include "BaseScene.h"

class ViewerScene :
    public BaseScene
{
public:
    ViewerScene();
    ~ViewerScene();
private:
    // �t�B�[���h�̊i�q����
    static constexpr int rayNum = 30;
    // �ړ���
    static constexpr float moveValue = 10;

    // ���C�g�s��
    struct LightMatrix
    {
        MATRIX view;                                                // �r���[
        MATRIX projection;                                          // �ˉe
    };
    // �J�������
    struct Camera
    {
        static constexpr float ortPro = 2000.0f;                    // ���ˉe�̕\���͈�
        static constexpr float disNear = 0.001f;                    // ��O�̋���
        static constexpr float disFar = 4000.0f;                    // ���̋���
        Vector3 target;                                             // �����_
        Vector3 pos;                                                // ���W
        Vector3 rot;                                                // ��]
        Vector3 angle;                                              // �p�x
    };
    // ���f�����
    struct Model
    {
        int handle;                                                 // �O���t�B�b�N�n���h��
        Vector3 pos;                                                // ���W
        float angle;                                                // �p�x
        Vector3 size;                                               // �T�C�Y
        int mesh;                                                   // �e�`�惁�b�V��
    };
    // �X�e�[�W���
    struct Stage
    {
        int handle;                                                 // �O���t�B�b�N�n���h��
        Vector3 size;
        int mesh;                                                   // �e�`�惁�b�V��
    };
    // �|�X�g�v���Z�X
    struct PostProcess
    {
        bool flag;                                                  // �|�X�g�G�t�F�N�g���L����
        int tex;                                                    // �I�t�X�N���[���p�e�N�X�`��
        std::array<int, 10> ps;                                     // �s�N�Z���V�F�[�_
    };
    // �u���[
    struct Blur
    {
        int side;                                                   // ���u���[
        int vert;                                                   // �c�u���[
    };
    // �萔�o�b�t�@
    struct CBuffer
    {
        int ps;                                                     // �s�N�Z��
        int vs;                                                     // ���_
    };
    // �V���h�[�}�b�v
    struct ShadowMap
    {
        int ps;                                                     // �V���h�[�s�N�Z��
        std::array<int, 6> vs;                                      // �V���h�[���_
        int setPS;                                                  // �s�N�Z���Z�b�g
        int tex;                                                    // �e�e�N�X�`��
    };
    // �s�N�Z�����
    struct Pixel
    {
        int toon;                                                   // �A�j����
        int lam;                                                    // �g�U����
        int tex;                                                    // �e�N�X�`��
    };

    // ������
    bool Init(void) override;
    // �J������񏉊���
    void InitCamera(void);
    // ���f���֌W������
    void InitModel(void);
    // ���C�g������
    void InitLight(void);
    // �e�֌W������
    void InitShadow(void);
    // �|�X�g�v���Z�X������
    void InitPostProcess(void);
    // �g�[���}�b�v������
    void InitToonMap(void);
    // �V�F�[�_�[������
    void InitShader(int model);

    // �X�V
    SceneUptr Update(SceneUptr ownScene, float delta) override;
    // �`��
    void DrawOwnScreen(float delta) override;
    // �V�[��ID�擾
    SceneID GetSceneID(void) override { return SceneID::Viewer; }
    // ���
    void Relese(void);

    // ���f���ړ�����
    void ProcessMove(void);
    // ���f����]����
    void ProcessRotate(void);

    // �e�p�̐[�x�L�^�摜����
    void CreateShadowMap(void);
    // ���f���`��(�V���h�[�}�b�v����)
    void DrawOffScreen(void);

    // �|�X�g�v���Z�X���s
    void DrawPostProcess(Vector2 pos, int offScreen, int shader, bool flag);
    // ���_���i�[
    VERTEX2DSHADER StorageVertex(VERTEX2DSHADER vert, Vector2 pos, Vector2 texPos);

    // �e�\���p�̕ϐ�
    LightMatrix* lightMat_;
    LightMatrix lightM_;
    // �J����
    Camera camera_;
    // ���f��
    Model model_;
    // �X�e�[�W
    Stage stage_;
    // �V�F�[�_
    Pixel pixel_;           // �s�N�Z���V�F�[�_
    int vs_;                // ���_�V�F�[�_
    int toonMap_;           // �g�D�[���}�b�v(�����������ĂȂ��ʂ��Â�����)
    // �V���h�E�}�b�v�̍쐬
    ShadowMap shadowMap_;
    // �|�X�g�G�t�F�N�g�p
    PostProcess post_;
    // ��ʊE�[�x�p
    Blur blur_;
    // �萔�o�b�t�@
    CBuffer cBuf_;

    // �A�j���[�V����
    int attachIndex_;
    float totalTime_;
    float playTime_;

    // ��]����
    float rot_;         // ��]��
};

