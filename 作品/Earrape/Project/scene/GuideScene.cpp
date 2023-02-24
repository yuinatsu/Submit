#include <DxLib.h>
#include "GuideScene.h"
#include "../FilePass.h"
#include "../input/Mouse.h"
#include "../_debug/_DebugConOut.h"

GuideScene::GuideScene()
{
}

GuideScene::~GuideScene()
{
}

SCN_ID GuideScene::Update(char* keyData, char* keyDataOld)
{
	//TRACE("GuideSceneの更新\n");
	lpMouse.Update();

	if (lpMouse.GetPresCnt(Mouse::RMB) == 1)
	{
		// 右クリックされた瞬間
		if (pageCnt_ < static_cast<int>(GuidePage::MAX))
		{
			pageCnt_++;
			PlaySoundMem(LoadSoundMem(GUIDE_BOOK_SE1), DX_PLAYTYPE_BACK, true);

		}
		else
		{
			PlaySoundMem(LoadSoundMem(GUIDE_BOOK_SE3), DX_PLAYTYPE_BACK, true);

			return SCN_ID::TITLE;

		}
	}
	if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
	{
		// 左クリックされた瞬間
		if (pageCnt_ > 1)
		{
			pageCnt_--;
			PlaySoundMem(LoadSoundMem(GUIDE_BOOK_SE1), DX_PLAYTYPE_BACK, true);

		}
	}

	return SCN_ID::GUIDE;
}

void GuideScene::Draw(void)
{
	DrawGraph(0, 0, guideImage_[(pageCnt_ - 1)], true);
}

bool GuideScene::Init(void)
{
	pageCnt_ = 1;

	guideImage_[0] = LoadGraph(GH_GUIDE_1);
	guideImage_[1] = LoadGraph(GH_GUIDE_2);
	guideImage_[2] = LoadGraph(GH_GUIDE_3);
	guideImage_[3] = LoadGraph(GH_GUIDE_4);
	guideImage_[4] = LoadGraph(GH_GUIDE_5);
	return true;
}

bool GuideScene::Release(void)
{
	for (int num = 0; num < static_cast<int>(GuidePage::MAX); num++)
	{
		DeleteGraph(guideImage_[num]);
	}
	return true;
}
