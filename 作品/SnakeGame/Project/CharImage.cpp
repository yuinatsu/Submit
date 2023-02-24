#include <DxLib.h>
#include <string>
#include "CharImage.h"
#include "_debug/_DebugConOut.h"

// CharImageの生成
//------------------
CharImage::CharImage()
{
}

// CharImageの破棄
//------------------
CharImage::~CharImage()
{
}

// CharImageの初期化
//--------------------
bool CharImage::Init(CHAR_ID charID)
{
	bool rtnFlag = true;
	if (charID >= CHAR_ID::DRAW)																														// CHAR_IDがREIMU~YOUMUの間でなければ読み込まない
	{
		return false;
	}

	std::string charNameList[static_cast<int>(CHAR_ID::MAX)] = {																						// キャラ名を格納
		"reimu",
		"marisa",
		"cirno",
		"sakuya",
		"youmu",
		""
	};

	if ((LoadDivGraph(("Resource/image/" + charNameList[static_cast<int>(charID)] + "_chip.png").c_str(), 
		ANIM_IMAGE_MAX * static_cast<int>(DIR::MAX), ANIM_IMAGE_MAX, static_cast<int>(DIR::MAX), 
		CHAR_SIZE_X, CHAR_SIZE_Y, &chipImage[0][0])) == -1)						// キャラチップ
	{
		TRACE("読み込みエラー1\n");
		rtnFlag = false;
	}
	if ((iconImage_ = LoadGraph(("Resource/image/" + charNameList[static_cast<int>(charID)] + "_icon.png").c_str())) == -1)								// キャラアイコン
	{
		TRACE("読み込みエラー2\n");
		rtnFlag = false;
	}
	if ((LoadDivGraph("Resource/image/picture.png", CHAR_PIC_MAX, CHAR_PIC_MAX, 1, CHAR_PIC_SIZE_X, CHAR_PIC_SIZE_Y, &picImage[0][0])) == -1)			// キャラ立ち絵
	{
		TRACE("読み込みエラー3\n");
		rtnFlag = false;
	}

	charID_ = charID;																																	// 送られてきたcharIDをcharID_に格納
	return rtnFlag;
}

// CharImageの解放
//------------------
bool CharImage::Release(void)
{
	for (int no = 0; no < static_cast<int>(CHAR_ID::MAX); no++)			// picImageを解放
	{
		DeleteGraph(picImage[0][no]);
	}

	DeleteGraph(iconImage_);											// iconImageの解放

	for (int dir = 0; dir < static_cast<int>(DIR::MAX); dir++)			// 分割して読んだchipImageを全解放
	{
		for (int anim = 0; anim < ANIM_IMAGE_MAX; anim++)
		{
			DeleteGraph(chipImage[dir][anim]);
		}
	}
	return true;
}
