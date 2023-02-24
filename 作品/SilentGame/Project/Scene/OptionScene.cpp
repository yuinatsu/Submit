#include <DxLib.h>
#include "OptionScene.h"
#include "PauseScene.h"
#include "../common/ImageMng.h"
#include "../Input/InputConfig.h"
#include "../common/SoundPross.h"
#include "../common/MenuUI/MenuUICtrl.h"
#include "../common/Option/KeyConfigOption.h"
#include "GameScene.h"
#include "../common/Option/SoundOption.h"

constexpr char key[]{ "./Resource/Input/Keydata.png" };
constexpr char pad[]{ "./Resource/Input/Paddata.png" };
constexpr char padPS[]{ "./Resource/Input/PaddataPS.png" };
constexpr char num[]{ "./Resource/Image/Option/number.png" };

// メニューウィンドウのサイズ
constexpr Math::Vector2I menuWndSize{ 940,410 };

// メニューウィンドウの左上座標
constexpr Math::Vector2I menuWndPos{ 70,140 };

// メニュー開閉までの時間
constexpr float menuActTime{ 0.5f };

OptionScene::OptionScene(uniqueScene gameScene)
{
	beforeScene_ = std::move(gameScene);
	wndScreenH_ = MakeScreen(menuWndSize.x, menuWndSize.y, true);
	
	isBeforGameScene_ = false;
	Init();
	DrawOwnScreen(0.0);
}

OptionScene::OptionScene(uniqueScene gameScene, uniqueScene beforScene, int mapNum,Scene scene)
{
	scene_ = scene;
	mapNum_ = mapNum;
	// beforScene_ = std::move(beforScene);
	beforeScene_ = std::move(gameScene);
	isBeforGameScene_ = true;
	wndScreenH_ = MakeScreen(menuWndSize.x, menuWndSize.y, true);
	Init();
	DrawOwnScreen(0.0);
}

OptionScene::~OptionScene()
{
}

bool OptionScene::Init(void)
{
	lpImageMng.GetID("Resource/Image/Option/OptionFrame.png", "OptionFrame");
	lpImageMng.GetID("Resource/Image/Option/number.png", "volumeNumber", Vector2I(15, 25), Vector2I(10, 1));
	lpImageMng.GetID("./Resource/Input/Keydata.png", "Keydata", Vector2I(75, 25), Vector2I(10, 6));
	auto type = GetJoypadType(DX_INPUT_PAD1);
	if (type == 3 || type == 4)
	{
		lpImageMng.GetID("./Resource/Input/PaddataPS.png", "PadPS", Vector2I(90, 25), Vector2I(10, 2));
	}
	else
	{
		lpImageMng.GetID("./Resource/Input/Paddata.png", "PadXbox", Vector2I(75, 25), Vector2I(10, 1));
	}

	// メニューUIを追加
	menuUICtrl_ = std::make_unique<MenuUICtrl<OSELECT>>(*controller_, static_cast<int>(OSELECT::Max),3.0f,OSELECT::Sound);
	menuUICtrl_->AddMenu(OSELECT::Sound, std::make_unique<MenuUI>(Math::Vector2{ 150.0f,80.0f },"Resource/Image/Option/Volume.png"));
	menuUICtrl_->AddMenu(OSELECT::Key, std::make_unique<MenuUI>(Math::Vector2{ 150.0f,180 }, "Resource/Image/Option/keyconfig.png"));
	menuUICtrl_->AddMenu(OSELECT::Exit, std::make_unique<MenuUI>(Math::Vector2{ 150.0f,280.0f }, "Resource/Image/Option/Exit.png"));
	update_ = &OptionScene::UpdateOpen;
	draw_ = &OptionScene::DrawOpen;
	return true;
}

uniqueScene OptionScene::Update(float delta, uniqueScene ownScene)
{
	time_ += delta;
	DrawOwnScreen(delta);
	controller_->Update(delta);
	return (this->*update_)(delta, std::move(ownScene));
}

void OptionScene::DrawOwnScreen(float delta)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	(this->*draw_)();
}



uniqueScene OptionScene::UpdateSetting(float delta, uniqueScene ownScene)
{
	if (!optionWnd_->Update(delta))
	{
		// Updateがfalseの時設定項目から戻る
		optionWnd_.reset();
		update_ = &OptionScene::UpdateSelect;
		draw_ =&OptionScene::DrawSelect;
	}
	return ownScene;
}

uniqueScene OptionScene::UpdateSelect(float delta, uniqueScene ownScene)
{
	menuUICtrl_->Update(delta);
	if (controller_->Pressed(InputID::Attack) && time_ > 0.2f)
	{
		if (menuUICtrl_->GetNowMenu() == OSELECT::Sound)
		{
			// サウンドの設定の時
			optionWnd_ = std::make_unique<SoundOption>(*controller_);
			update_ = &OptionScene::UpdateSetting;
			draw_ = &OptionScene::DrawSetting;
			time_ = 0.0f;
		}
		else if (menuUICtrl_->GetNowMenu() == OSELECT::Key)
		{
			// キーコンフィグの時
			optionWnd_ = std::make_unique<KeyConfigOption>(*controller_);
			update_ = &OptionScene::UpdateSetting;
			draw_ = &OptionScene::DrawSetting;
			time_ = 0.0f;
		}
		else
		{
			// 戻る時
			update_ = &OptionScene::UpdateClose;
			draw_ = &OptionScene::DrawClose;
			time_ = 0.0f;
		}
	}
	return ownScene;
}

uniqueScene OptionScene::UpdateOpen(float delta, uniqueScene ownScene)
{
	time_ += delta;
	
	if (time_ >= menuActTime)
	{
		update_ = &OptionScene::UpdateSelect;
		draw_ = &OptionScene::DrawSelect;
		time_ = 0.0f;
	}

	return ownScene;
}

uniqueScene OptionScene::UpdateClose(float delta, uniqueScene ownScene)
{
	time_ += delta;

	if (time_ >= menuActTime)
	{
		update_ = &OptionScene::UpdateSelect;
		draw_ = &OptionScene::DrawSelect;
		time_ = 0.0f;
		Relese();
		if (!isBeforGameScene_)
		{
			beforeScene_->SoundPlay();
			return std::move(beforeScene_);
		}
		else
		{
			bool isSmallMap{true};
			if (beforeScene_->GetSceneID() == Scene::Game)
			{
				isSmallMap = dynamic_cast<GameScene&>(*beforeScene_).IsSmallMap();
			}
			return std::make_unique<PauseScene>(std::move(beforeScene_), mapNum_,isSmallMap, scene_);
		}
	}

	return ownScene;
}

void OptionScene::Relese()
{
	lpImageMng.Relese("volumeNumber");
	DeleteGraph(wndScreenH_);
}

void OptionScene::DrawSetting(void)
{
	// メニュー用スクリーンに描画
	SetDrawScreen(wndScreenH_);
	ClsDrawScreen();
	DrawMenu();
	if (optionWnd_->IsDrawInput())
	{
		DrawInput();
	}

	SetDrawScreen(screenID_);
	beforeScene_->Draw(0.0f);
	DrawModiGraph(
		menuWndPos.x, menuWndPos.y,
		menuWndPos.x + menuWndSize.x, menuWndPos.y,
		menuWndPos.x + menuWndSize.x,menuWndPos.y + menuWndSize.y,
		menuWndPos.x, menuWndPos.y + menuWndSize.y,
		wndScreenH_, true
	);
	
	optionWnd_->Draw();
	
}

void OptionScene::DrawSelect(void)
{
	// メニュー用スクリーンに描画
	SetDrawScreen(wndScreenH_);
	ClsDrawScreen();
	DrawMenu();
	DrawInput();
	SetDrawScreen(screenID_);

	// 背景を表示
	beforeScene_->Draw(0.0f);

	// メニューを表示
	DrawModiGraph(
		menuWndPos.x, menuWndPos.y,
		menuWndPos.x + menuWndSize.x, menuWndPos.y,
		menuWndPos.x + menuWndSize.x, menuWndPos.y + menuWndSize.y,
		menuWndPos.x, menuWndPos.y + menuWndSize.y,
		wndScreenH_, true
	);
}

void OptionScene::DrawOpen(void)
{
	// メニュー用スクリーンに描画
	SetDrawScreen(wndScreenH_);
	ClsDrawScreen();
	DrawMenu();
	DrawInput();
	SetDrawScreen(screenID_);

	// 背景の表示
	beforeScene_->Draw(0.0f);

	// メニューを表示
	float w = time_ / menuActTime;
	w = 1.0f - w;
	DrawModiGraph(
		menuWndPos.x, menuWndPos.y + static_cast<int>(menuWndSize.y / 2 * w),
		menuWndPos.x + menuWndSize.x, menuWndPos.y + static_cast<int>(menuWndSize.y / 2 * w),
		menuWndPos.x + menuWndSize.x, (menuWndPos.y + menuWndSize.y) - static_cast<int>(menuWndSize.y / 2 * w),
		menuWndPos.x, (menuWndPos.y + menuWndSize.y) - static_cast<int>(menuWndSize.y / 2 * w),
		wndScreenH_, true
	);
}

void OptionScene::DrawClose(void)
{
	// メニュー用スクリーンに描画
	SetDrawScreen(wndScreenH_);
	ClsDrawScreen();
	DrawMenu();
	DrawInput();
	SetDrawScreen(screenID_);

	// 背景の表示
	beforeScene_->Draw(0.0f);

	// メニューを表示
	float w = time_ / menuActTime;
	DrawModiGraph(
		menuWndPos.x, menuWndPos.y + static_cast<int>(menuWndSize.y / 2 * w),
		menuWndPos.x + menuWndSize.x, menuWndPos.y + static_cast<int>(menuWndSize.y / 2 * w),
		menuWndPos.x + menuWndSize.x, (menuWndPos.y + menuWndSize.y) - static_cast<int>(menuWndSize.y / 2 *w),
		menuWndPos.x, (menuWndPos.y + menuWndSize.y) - static_cast<int>(menuWndSize.y / 2 * w),
		wndScreenH_, true
	);
}

void OptionScene::DrawInput(void)
{
	auto& keydata = lpImageMng.GetID("Keydata");
	auto type = GetJoypadType(DX_INPUT_PAD1);
	std::string paddata{ "PadXbox" };
	if (type == 3 || type == 4)
	{
		paddata = { "PadPS" };
	}
	auto cntType = controller_->GetCntType();
	DrawGraph(530, 385, lpImageMng.GetID("./Resource/Image/Option/select.png")[0], true);
	if (cntType == CntType::Key)
	{
		DrawGraph(705, 385, lpImageMng.GetID("./Resource/Image/Option/decision.png")[0], true);
		DrawGraph(780, 385, keydata[lpConfigMng.GetKeyInputCode().at(InputID::Attack)], true);
	}
	else
	{
		DrawGraph(705, 385, lpImageMng.GetID("./Resource/Image/Option/decision.png")[0], true);
		DrawGraph(780, 385, lpImageMng.GetID(paddata)[lpConfigMng.GetJoypadInputCode().at(InputID::Attack)], true);
	}
}

void OptionScene::DrawMenu(void)
{
	
	/*SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, 920, 410, 0x000000, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);*/
	DrawGraph(0, 0, lpImageMng.GetID("OptionFrame")[0], true);
	menuUICtrl_->Draw();

}
