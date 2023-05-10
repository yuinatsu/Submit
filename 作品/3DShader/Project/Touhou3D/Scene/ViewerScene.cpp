#include <DxLib.h>
#include <array>
#include <cassert>
#include "ViewerScene.h"
#include "../Common/ImageMng.h"
#include "../Common/Debug.h"

ViewerScene::ViewerScene()
{
    // ������
    Init();
}

ViewerScene::~ViewerScene()
{
    // ���
    Relese();
}

bool ViewerScene::Init(void)
{
    // ���f���֌W������
    InitModel();
    // �J����������
    InitCamera();
    // ���C�g������
    InitLight();
    // �e�֌W������
    InitShadow();
    // �|�X�g�v���Z�X������
    InitPostProcess();
    // �g�[���p�̉摜���Z�b�g
    InitToonMap();
    // �V�F�[�_�[������
    InitShader(model_.handle);

    attachIndex_ = MV1AttachAnim(model_.handle, 0, -1, false);
    totalTime_ = MV1GetAttachAnimTotalTime(model_.handle, attachIndex_);
    playTime_ = 0.0f;

    return true;
}

void ViewerScene::InitCamera(void)
{
    // �J����
    camera_.target = model_.pos + Vector3(0.0f, 150.0f, -100.0f);
    camera_.pos = zeroVector3<float>;
    camera_.rot = zeroVector3<float>;
    camera_.angle = zeroVector3<float>;
}

void ViewerScene::InitModel(void)
{
    // ���f��
    model_.handle = MV1LoadModel("./Resource/Model/�\�Z����/sakuya.mv1");
    model_.pos = Vector3(0.0f, 100.0f, -100.0f);
    model_.angle = Deg2Rad(180.0f);
    model_.size = Vector3(10.0f, 10.0f, 10.0f);
    // �X�e�[�W
    stage_.handle = MV1LoadModel("./Resource/Stage/�l��/�l��.mv1");
    stage_.size = Vector3(100.0f, 100.0f, 100.0f);
}

void ViewerScene::InitLight(void)
{
    // �f�t�H���g���C�g�ݒ�
    SetLightDirection(VGet(1.0f, -1.0f, 1.0f));                 // ����
    SetLightDifColor(GetColorF(1.0f, 1.0f, 1.0f, 1.0f));        // �f�B�t���[�Y�J���[
    SetLightSpcColor(GetColorF(1.0f, 1.0f, 1.0f, 1.0f));        // �X�y�L�����J���[
    SetLightAmbColor(GetColorF(0.5f, 0.5f, 0.5f, 1.0f));        // �A���r�G���g�J���[

    // ���C�g�s�񏉊���
    lightM_.view = MGetIdent();
    lightM_.projection = MGetIdent();

    // �萔�o�b�t�@(�V�F�[�_�[)
    cBuf_.vs = CreateShaderConstantBuffer(sizeof(LightMatrix) * 4);                    // �n���h��������
    lightMat_ = static_cast<LightMatrix*>(GetBufferShaderConstantBuffer(cBuf_.vs));    // �萔�o�b�t�@�̃A�h���X�擾
}

void ViewerScene::InitShadow(void)
{
    shadowMap_.vs[0] = LoadVertexShader("Resource/Shader/ShadowMap/Mesh1Shadow.vso");
    shadowMap_.vs[1] = LoadVertexShader("Resource/Shader/ShadowMap/Mesh4Shadow.vso");
    shadowMap_.ps = LoadPixelShader("Resource/Shader/ShadowMap/ShadowMapPS.pso");
    shadowMap_.setPS = LoadPixelShader("Resource/Shader/Shadow/SetShadowMap.pso");

    // �X�e�[�W
    stage_.mesh = LoadVertexShader("Resource/Shader/Shadow/StageVS.vso");
    // ���f��
    model_.mesh = LoadVertexShader("Resource/Shader/Shadow/ModelVS.vso");

    // �V���h�[�}�b�v����
    SetDrawValidFloatTypeGraphCreateFlag(true);     // ���������_�^
    SetCreateDrawValidGraphChannelNum(1);           // 1�`�����l��
    SetCreateGraphColorBitDepth(16);                // 16�r�b�g
    shadowMap_.tex = MakeScreen(screenSize_.x, screenSize_.y, false);           // �摜����
    // �ݒ�����ɖ߂�
    SetDrawValidFloatTypeGraphCreateFlag(false);    // �����^
    SetCreateDrawValidGraphChannelNum(4);           // 4�`�����l��
    SetCreateGraphColorBitDepth(32);                // 32�r�b�g
}

void ViewerScene::InitPostProcess()
{
    // �|�X�g�v���Z�X
    post_.flag = false;
    post_.tex = MakeScreen(screenSize_.x, screenSize_.y);                      // �|�X�g�v���Z�X�p�I�t�X�N���[��
    post_.ps[0] = LoadPixelShader("Resource/Shader/PostEffect/Aveblur.pso");            // �u���[
    post_.ps[1] = LoadPixelShader("Resource/Shader/PostEffect/Mono.pso");               // ���m�g�[���J���[
    // ��ʊE�[�x
    blur_.side = MakeScreen(screenSize_.x / 2, screenSize_.y, false);          // ���u���[
    blur_.vert = MakeScreen(screenSize_.x / 2, screenSize_.y / 2, false);      // �c�u���[
}

void ViewerScene::InitToonMap(void)
{
    toonMap_ = LoadGraph("./Resource/Model/ToonMap.png");
    // �g�[���}�b�v�Z�b�g
    MV1SetPosition(model_.handle, VGet(model_.pos.x, model_.pos.y, model_.pos.z));
    MV1SetScale(model_.handle, VGet(model_.size.x, model_.size.y, model_.size.z));
    MV1SetPosition(stage_.handle, VGet(0.0f, 0.0f, 0.0f));
    MV1SetScale(stage_.handle, VGet(stage_.size.x, stage_.size.y, stage_.size.z));
}

void ViewerScene::InitShader(int model)
{
    int vertType = -1;
    for (int i = 0; i < MV1GetTriangleListNum(model); ++i)
    {
        vertType = MV1GetTriangleListVertexType(model, i);        // ���f���̃g���C�A���O�����X�g�̒��_�f�[�^�^�C�v���擾
        break;
    }
    switch (vertType)
    {
    case DX_MV1_VERTEX_TYPE_1FRAME : 
        vs_ = LoadVertexShader("Resource/Shader/Vertex/Mesh1.vso");
        break;
    case DX_MV1_VERTEX_TYPE_4FRAME:
        vs_ = LoadVertexShader("Resource/Shader/Vertex/Mesh4.vso");
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_1FRAME:
        vs_ = LoadVertexShader("Resource/Shader/Vertex/NormMesh1.vso");
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_4FRAME:
        vs_ = LoadVertexShader("Resource/Shader/Vertex/NormMesh4.vso");
        break;
    default:
        break;
    }
    // �@���}�b�v�p�̏�񂪊܂܂�Ă��邩
    if (vertType <= DX_MV1_VERTEX_TYPE_NMAP_1FRAME)
    {
        pixel_.tex = LoadPixelShader("Resource/Shader/Pixel/Tex.pso");
        pixel_.lam = LoadPixelShader("Resource/Shader/Pixel/Lambert.pso");
        pixel_.toon = LoadPixelShader("Resource/Shader/Pixel/Toon.pso");
    }
    else
    {
        pixel_.tex = LoadPixelShader("Resource/Shader/Pixel/NormTex.pso");
        pixel_.lam = LoadPixelShader("Resource/Shader/Pixel/NormLambert.pso");
        pixel_.toon = LoadPixelShader("Resource/Shader/Pixel/NormToon.pso");
    }

    // �V�F�[�_�[�ݒ�
    SetUseVertexShader(vs_);                // ���_�V�F�[�_�[�ݒ�
    SetUsePixelShader(pixel_.tex);          // �s�N�Z���V�F�[�_�[�ݒ�
    // �g�[���}�b�v
    SetDrawMode(DX_DRAWMODE_BILINEAR);      // �g�[���}�b�v�̉掿�򉻂�h�����߂Ƀo�C���j�A�@�ŕ`��
    SetUseTextureToShader(3, toonMap_);     // �V�F�[�_�[�`��Ŏg�p����O���t�B�b�N�n���h���ݒ�
    // Z�o�b�t�@�ݒ�
    SetUseZBuffer3D(true);                  // �g�p�ł���悤�ɂ���
    SetWriteZBuffer3D(true);                // �������݂��s����悤�ɂ���
}

SceneUptr ViewerScene::Update(SceneUptr ownScene, float delta)
{
    controller_->Update(delta);
    rot_ = controller_->GetLeftInput().GetAngle();
    // ���f���ړ�
    ProcessMove();
    // ���f����]
    ProcessRotate();

    // �J������]
    if (controller_->Press(InputID::CameraRight))
    {
        camera_.rot.z += 100.0f;
    }
    if (controller_->Press(InputID::CameraLeft))
    {
        camera_.rot.z -= 100.0f;
    }

    // ���f��
    MV1SetPosition(model_.handle, VGet(model_.pos.x, model_.pos.y, model_.pos.z));      // ���W
    MV1SetScale(model_.handle, VGet(model_.size.x, model_.size.y, model_.size.z));      // �T�C�Y
    MV1SetRotationXYZ(model_.handle, VGet(0, model_.angle, 0));                         // ��]
    // �J����
    VECTOR offset = VTransform(VGet(0, 100, -500), MMult(MGetRotX(camera_.rot.x), MGetRotY(camera_.rot.y)));          // �s����g�������W�v�Z
    camera_.pos = camera_.angle + camera_.target;                                          // �J�������W(�v���C���[�ɓ���)


    if (CheckHitKey(KEY_INPUT_1))       post_.flag = true;                                 // �|�X�g�v���Z�X�L��
    if (CheckHitKey(KEY_INPUT_2))       post_.flag = false;                                // �|�X�g�v���Z�X����
    if (CheckHitKey(KEY_INPUT_3))       SetUsePixelShader(pixel_.tex);
    if (CheckHitKey(KEY_INPUT_4))       SetUsePixelShader(pixel_.lam);
    if (CheckHitKey(KEY_INPUT_5))       SetUsePixelShader(pixel_.toon);

    // �A�j���[�V����
    playTime_ += 0.8f;
    if (playTime_ >= totalTime_)
    {
        playTime_ = 0.0f;
    }
    MV1SetAttachAnimTime(model_.handle, attachIndex_, playTime_);

    DrawOwnScreen(delta);
    return ownScene;
}

void ViewerScene::DrawOwnScreen(float delta)
{
    // �e�p�[�x�摜����
    CreateShadowMap();
    // ���f���̕`��(�V���h�[�}�b�v����)
    DrawOffScreen();
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    // �|�X�g�v���Z�X
    DrawPostProcess(Vector2(0, 0), post_.tex, post_.ps[0], post_.flag);
    DrawFormatString(10, 10, 0xffffffff, "FPS=%f", GetFPS());
    // �V���h�E�}�b�v(������)
    //DrawRotaGraph(133, 83, 0.25, 0.0, shadowMap_.tex, true);
    ScreenFlip();
}

void ViewerScene::Relese(void)
{
}

void ViewerScene::ProcessMove(void)
{
    // �ړ���
    auto move = moveValue;
    if (controller_->Press(InputID::Dash))          move *= 3;

    // �ʏ�ړ�
    if (controller_->GetLeftInput().SqMagnitude() > 0.0f)
    {
        auto camAngle = ToEuler(camera_.rot);
        model_.pos.x -= sinf(camAngle.y + rot_) * move;
        model_.pos.z -= cosf(camAngle.y + rot_) * move;
        camera_.target.x -= sinf(camAngle.y + rot_) * move;
        camera_.target.z -= cosf(camAngle.y + rot_) * move;
    }
}

void ViewerScene::ProcessRotate(void)
{
    // ���f���̊p�x��ݒ�
    if (!(controller_->GetLeftInput().SqMagnitude() > 0.0f))
    {
        return;
    }

    // �ړ������ɃL�����N�^�[�̊p�x�����X�ɕς���
    float radUnitAnglesY = model_.angle;
    auto camAngle = ToEuler(camera_.rot);

    float radMoveAnglesY = RadIn2PI(camAngle.y + rot_);

    // ��]�ʂ����Ȃ����̉�]�������擾����i���v���F�P�A�����v���F�[�P�j
    float aroundDir = DirNearAroundRad(radUnitAnglesY, radMoveAnglesY);

    // true:�������l�ȓ��ɍ��킹�� false:��]�ʂ�������
    float diff = abs(radMoveAnglesY - radUnitAnglesY);
    auto rotRad = 5.0f * (DX_PI_F / 180.0f);
    model_.angle = (diff < 0.1f) ? radMoveAnglesY : model_.angle + (rotRad * aroundDir);

    // angles_.y��0~360�x�ȓ��Ɏ��߂�
    model_.angle = RadIn2PI(model_.angle);
}

void ViewerScene::CreateShadowMap(void)
{
    // �`�����e�p�[�x�L�^�摜�ɂ���
    SetDrawScreen(shadowMap_.tex);
    // �e�p�[�x�L�^�摜��^�����ɂ���
    SetBackgroundColor(255, 255, 255);
    ClearDrawScreen();
    SetBackgroundColor(0, 0, 0);

    // �J����
    SetupCamera_Ortho(camera_.ortPro);                      // �J�����̃^�C�v�𐳎ˉe�^�C�v�Z�b�g�A�`��͈͂��w��
    SetCameraNearFar(camera_.disNear, camera_.disFar);      // �`�悷�鉜�s�͈͂��Z�b�g
    auto lightPos = VAdd(VGet(camera_.target.x, camera_.target.y, camera_.target.z), VScale(GetLightDirection(), -2000));        // ���C�g���W
    SetCameraPositionAndTarget_UpVecY(lightPos, VGet(camera_.target.x, camera_.target.y, camera_.target.z));                     // �J�����̎��_�A�����_�̐ݒ�

    // �V�F�[�_�[
    MV1SetUseOrigShader(true);                                          // ���f���̕`��� SetUseVertexShader, SetUsePixelShader ���g�p�ł���悤�ɂ���
    // �[�x�l�s�N�Z���V�F�[�_�[
    SetUsePixelShader(shadowMap_.ps);                                  // �[�x�l�ւ̕`��p�̃s�N�Z���V�F�[�_�[���Z�b�g
    SetUseVertexShader(shadowMap_.vs[0]);                              // �[�x�L�^�摜�ւ̍��̃��b�V���`��p�̒��_�V�F�[�_�[���Z�b�g
    // �X�e�[�W�`��
    MV1DrawModel(stage_.handle);
    // �[�x�n�L�^�摜�ւ̃X�L�j���O���b�V���`��p�̒��_�V�F�[�_�[���Z�b�g
    SetUseVertexShader(shadowMap_.vs[1]);
    // ���f���`��
    MV1DrawModel(model_.handle);
    MV1SetUseOrigShader(false);                                         // ���f���̕`��� SetUseVertexShader, SetUsePixelShader ���g�p�ł��Ȃ��悤�ɖ߂�
    // ���C�g�̍��W��������Ă���(�J���������C�g�̏��������Ă���)
    lightM_.view = GetCameraViewMatrix();
    lightM_.projection = GetCameraProjectionMatrix();
}

void ViewerScene::DrawOffScreen(void)
{
    SetDrawScreen(post_.tex);
    ClsDrawScreen();

    // �J�����ݒ�
    SetCameraPositionAndTarget_UpVecY(VGet(camera_.pos.x, camera_.pos.y + 100, camera_.pos.z - 300), VGet(camera_.target.x, camera_.target.y, camera_.target.z));

    // �V�F�[�_�[
    MV1SetUseOrigShader(true);                  // ���f���̕`��� SetUseVertexShader, SetUsePixelShader ���g�p�ł���悤�ɂ���
    // ���C�g
    lightMat_[0] = lightM_;
    // �萔�o�b�t�@
    UpdateShaderConstantBuffer(cBuf_.vs);                              // cbufferVS�̕ύX��K�p
    SetShaderConstantBuffer(cBuf_.vs, DX_SHADERTYPE_VERTEX, 4);        // cbufferVS�𒸓_�V�F�[�_�[�ɃZ�b�g
    SetUsePixelShader(shadowMap_.setPS);                               // �[�x�L�^��ʂ��g�����e�{�f�B���N�V���i�����C�g�P�`��p�̃s�N�Z���V�F�[�_���Z�b�g
    // �e�֌W
    SetUseTextureToShader(1, shadowMap_.tex);                          // �e�p�[�x�L�^�摜���e�N�X�`���P�ɃZ�b�g
    // �X�e�[�W�`��
    SetUseVertexShader(stage_.mesh);
    MV1DrawModel(stage_.handle);
    // ���f���`��
    SetUseVertexShader(model_.mesh);
    MV1DrawModel(model_.handle);
    MV1SetUseOrigShader(false);                  // ���f���̕`��� SetUseVertexShader, SetUsePixelShader ���g�p�ł��Ȃ��悤�ɖ߂�

    // �e�N�X�`������
    SetUseTextureToShader(1, -1);
    SetUseTextureToShader(3, -1);
}

void ViewerScene::DrawPostProcess(Vector2 pos, int offScreen, int shader, bool flag)
{
    if (flag)
    {
        std::array<VERTEX2DSHADER, 4> verts{};                  // 2D�`��Ɏg�p���钸�_�f�[�^
        for (auto& v : verts)
        {
            // ������
            v.pos.z = 0.0f;                                     // Z���W
            v.rhw = 1.0;                                        // ���� W �̋t��
            v.dif = GetColorU8(0xff, 0xff, 0xff, 0xff);         // �f�B�t���[�Y
            v.spc = GetColorU8(0xff, 0xff, 0xff, 0xff);		    // �X�y�L����
            v.su = 0.0f;                                        // �⏕�e�N�X�`�����������W
            v.sv = 0.0f;                                        // �⏕�e�N�X�`���c�������W
        }

        // ��ʃT�C�Y�擾
        int width, height;
        GetGraphSize(offScreen, &width, &height);
        // ���_�V�F�[�_�[���i�[
        verts[0] = StorageVertex(verts[0], pos, Vector2(0.0f, 0.0f));                                       // ����
        verts[1] = StorageVertex(verts[1], Vector2(pos.x + width, pos.y), Vector2(1.0f, 0.0f));             // �E��
        verts[2] = StorageVertex(verts[2], Vector2(pos.x, pos.y + height), Vector2(0.0f, 1.0f));            // ����
        verts[3] = StorageVertex(verts[3], Vector2(pos.x + width, pos.y + height), Vector2(1.0f, 1.0f));    // �E��

        SetUsePixelShader(shader);                                                                          // �s�N�Z���V�F�[�_�[�ݒ�
        SetUseTextureToShader(0, offScreen);                                                                // �V�F�[�_�[�n���h���ݒ�
        DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);                     // �V�F�[�_�[���g����2D�v���~�e�B�u��`��
    }
    else
    {
        // �|�X�g�v���Z�X�Ȃ��̃I�t�X�N���[��
        DrawGraph(pos.x, pos.y, offScreen, true);
    }
    // �e�N�X�`������
    SetUseTextureToShader(0, -1);
}

VERTEX2DSHADER ViewerScene::StorageVertex(VERTEX2DSHADER vert, Vector2 pos, Vector2 texPos)
{
    vert.pos.x = pos.x;
    vert.pos.y = pos.y;
    vert.u = texPos.x;
    vert.v = texPos.y;
    return vert;
}
