#include <DxLib.h>
#include "CircleScene.h"

constexpr int ScreenSizeX = 1060;
constexpr int ScreenSizeY = 660;

CircleScene::CircleScene(double limitTime, uniqueScene beforScene, uniqueScene afterScene):
    TransitionScene(std::move(beforScene), std::move(afterScene))
{
	limitTime_ = limitTime;
	DrawOwnScreen(0.0);
}

CircleScene::~CircleScene()
{

}

bool CircleScene::UpdataTransition(double delta)
{
	count += delta;
	// limitTime�b�o�����玟�̃V�[���ֈȍ~
	// �g�����W�V�������I����Ă����true
	if (count > limitTime_)
	{
		return true;
	}
	// �I����Ă��Ȃ����false
	return false;
}

void CircleScene::DrawOwnScreen(float delta)
{
	// �������g�̃X�N���[���ɑ΂���Draw
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	auto alpha = static_cast<int>(255 * count / limitTime_);
	afterScene_->Draw(delta);
	DrawReversalCircle(ScreenSizeX / 2, ScreenSizeY / 2, static_cast<int>(550 * count / limitTime_), 0x000000);
}

int CircleScene::DrawReversalCircle(int x, int y, int r, int Color)
{
	// �~���]�`��̈�̊O����`��
	DrawBox(0, 0, 1060, y - r, Color, true);
	DrawBox(0, y - r, x - r, 660, Color, true);
	DrawBox(x - r, y + r + 1, 1060, 660, Color, true);
	DrawBox(x + r, y - r, 1060, y + r + 1, Color, true);
	return 0;
}

