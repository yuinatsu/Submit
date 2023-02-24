#include "DxLib.h"
#include "GameCommon.h"
#include "KeyCheck.h"
#include "SceneManager.h"
#include "Camera.h"
#include "TitleScene.h"
#include "SpaceDome.h"
#include "TextScroll.h"
#include "ParticleGenerator.h"
#include "AsoUtility.h"

TitleScene::TitleScene(SceneManager* manager) : SceneBase(manager)
{
}

void TitleScene::Init(void)
{

	mImgStartLogo = LoadGraph("Image/StartLogo.png");

	mSpaceDome = new SpaceDome(mSceneManager);
	mSpaceDome->Init();

	textScroll_ = new TextScroll(mSceneManager);
	textScroll_->Init();

	particle_ = new ParticleGenerator(mSceneManager, AsoUtility::VECTOR_ZERO, 10.0f);
	particle_->Init();
}

void TitleScene::Update(void)
{
	mSpaceDome->Update();
	textScroll_->Update();
	particle_->Update();
	// ƒV[ƒ“‘JˆÚ
	if (keyTrgDown[KEY_SYS_START])
	{
		mSceneManager->ChangeScene(SceneManager::SCENE_ID::GAME, true);
	}

}

void TitleScene::Draw(void)
{
	mSpaceDome->Draw();
	textScroll_->Draw();
	particle_->Draw();
	// ƒƒS•`‰æ
	DrawLogo();

}

void TitleScene::DrawLogo(void)
{

	int cx = SCREEN_SIZE_X / 2;
	int cy = SCREEN_SIZE_Y / 2;
	DrawRotaGraph(
		cx, cy - 200,
		1.0f, 0.0f, mImgStartLogo, true);

	char* msg = (char*)"Push Space";
	SetFontSize(28);
	int len = (int)strlen(msg);
	int width = GetDrawStringWidth(msg, len);
	DrawFormatString((SCREEN_SIZE_X / 2) - (width / 2), 200, 0x87cefa, msg);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);
	
}

void TitleScene::Release(void)
{
	mSpaceDome->Release();
	DeleteGraph(mImgStartLogo);
	textScroll_->Release();
	delete textScroll_;
	particle_->Release();
	delete particle_;
}
