#include <DxLib.h>
#include "SelectScene.h"
#include "LoadScene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "TutorialScene.h"
#include "OptionScene.h"
#include "Transition/CircleScene.h"
#include "Tutorial/PlayerTutorial.h"
#include "Tutorial/ItemTutorial.h"
#include "Tutorial//DoorTutorial.h"
#include "../Input/InputConfig.h"
#include "../Resource/Tmx/TmxObj.h"
#include "../common/MenuUI/MenuUICtrl.h"
#include "../common/SoundPross.h"
#include "../Object/ObjManager.h"
#include "../common/ImageMng.h"
//#include "../Shader/ShaderDrawGraph.h"

#include "../common/Debug.h"
#include "../common/MenuUI/MenuUI.h"

//�����̐��l�̓}�b�v���ɉ����đ��₷����
constexpr int num = 18;
constexpr char key[]{ "./Resource/Input/Keydata.png" };
constexpr char pad[]{ "./Resource/Input/Paddata.png" };
constexpr char padPS[]{ "./Resource/Input/PaddataPS.png" };

constexpr  float speed = 30.0f;

// �ړ���̍ő�ƍŏ�
constexpr float MoveMax = 1590.0f;
constexpr float MoveMin = -530.0f;

constexpr float StringPos = 480;

constexpr int OffsetY = 500;

constexpr  int selecterLPos = 300;

SelectScene::SelectScene()
{
	Init(); 
	DrawOwnScreen(0.0);
}

SelectScene::~SelectScene()
{
	//SoundProcess::Release();
}

void SelectScene::SoundPlay(void)
{
	SoundProcess::SoundPlay(SoundProcess::SOUNDNAME_BGM::select, true, loopTime_);
	loopTime_ = 0.0f;
}

bool SelectScene::Init(void)
{
	SetUseASyncLoadFlag(true);
	map_ = "tileset x1.png";
	// �Q�[���ɕK�v�ȏ�����
	InitGame();
	// �`�����̏�����
	InitScreen();
	SetUseASyncLoadFlag(false);
	return true;
}

void SelectScene::InitGame(void)
{
	LoadStageData();
	menuUICtrl_ = std::make_unique<MenuUICtrl<SELECT_TYPE3>>(*controller_, static_cast<int>(SELECT_TYPE3::Max), 3.0f, SELECT_TYPE3::Title);
	// ���E�ŃJ�[�\���𑀍삷��
	menuUICtrl_->SetMinusInputID(InputID::Left);
	menuUICtrl_->SetPlusInputID(InputID::Right);
	/*menuUICtrl_->SetMinusInputID(InputID::Up);
	menuUICtrl_->SetPlusInputID(InputID::Down);*/
	menuUICtrl_->AddMenu(SELECT_TYPE3::Title, std::make_unique<MenuUI>(Math::Vector2{static_cast<float>( ScreenSizeX / 2), static_cast<float>(ScreenSizeY / 2 + 280) }, "Resource/Image/SelectScene/Title.png"));
	menuUICtrl_->AddMenu(SELECT_TYPE3::Setting, std::make_unique<MenuUI>(Math::Vector2{ static_cast<float>(ScreenSizeX / 2 + 480), static_cast<float>(ScreenSizeY / 2 + 280) }, "Resource/Image/SelectScene/setting.png"));
	menuUICtrl_->SetFlag(false);
	tmxObj_ = std::make_shared<TmxObj>();
	time_ = 0.0f;
	loopTime_ = 0.0f;
	SceneFlag_ = false;
	noMap_ = false;
	selectType_ = static_cast<int>(SELECT_TYPE::TOne);
	selectType2_ = static_cast<int>(SELECT_TYPE2::StageSelect);
	moveMapR_ = true;
	moveMapL_ = false;
	moveMapOld_ = MoveMin;
	moveMap_ = mapPicPosX;

	SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_BGM::select, SoundProcess::GetBGMVolumeEntire(), true);
}

void SelectScene::InitScreen(void)
{

	// �}�b�v�摜�̓ǂݍ���
	lpImageMng.GetID("Resource/Image/SelectScene/Stage.png", "Stage", Math::Vector2I{ 576,326 }, Math::Vector2I{ 3,6 });

	// �}�b�v���Ɏg���摜
	lpImageMng.GetID("Resource/Image/SelectScene/EnemyCount.png", "EnemyCount");
	lpImageMng.GetID("Resource/Image/SelectScene/SecurityCamCount.png", "SecurityCamCount");
	lpImageMng.GetID("Resource/Image/SelectScene/SelectNumber.png", "SelectNumber", Math::Vector2I{20,20}, Math::Vector2I{10,1});
	lpImageMng.GetID("Resource/Image/SelectScene/MapSizeString.png", "MapSizeString");
	lpImageMng.GetID("Resource/Image/SelectScene/MapSize.png", "MapSize", Math::Vector2I{20,20}, Math::Vector2I{2,1});

	// �w�i
	lpImageMng.GetID("Resource/Image/TitleScene/Sprites/bg.png", "bg");
	lpImageMng.GetID("Resource/Image/TitleScene/Sprites/buildings.png", "buildings");
	lpImageMng.GetID("Resource/Image/TitleScene/Sprites/far-buildings.png", "far-buildings");
	lpImageMng.GetID("Resource/Image/TitleScene/Sprites/skill-foreground.png", "skill-foreground");
	lpImageMng.GetID("Resource/Image/fog/fog.png", "fog");
	lpImageMng.GetID("Resource/Image/fog/fogBG.png", "fogBG");

	lpImageMng.GetID("Resource/Image/SelectScene/StageString.png", "StageString", Math::Vector2I{ 300,35 }, Math::Vector2I{ 6,3 });

	selecterPic_ = lpImageMng.GetID("Resource/Image/SelectScene/selecter.png")[0];
	selecterLPic_ = lpImageMng.GetID("Resource/Image/SelectScene/selecter_L.png")[0];
	gameMap_ = lpImageMng.GetID("Resource/Image/SelectScene/gameMap.png")[0];
}

uniqueScene SelectScene::Update(float delta, uniqueScene ownScene)
{
	DrawOwnScreen(delta);
	controller_->Update(delta);
	return UpdateSetting(delta, std::move(ownScene));
}

uniqueScene SelectScene::UpdateSetting(float delta, uniqueScene ownScene)
{
	time_ += delta;
	if (time_ > 0.5f)
	{
		if (controller_->Pressed(InputID::Down))
		{
			if (selectType2_ < static_cast<int>(SELECT_TYPE2::Other))
			{
				SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::select, SoundProcess::GetSEVolumeEntire(), false);
				selectType2_ = (selectType2_ + 1) % static_cast<int>(SELECT_TYPE2::Max);
			}
		}
		if (controller_->Pressed(InputID::Up))
		{
			if (selectType2_ > static_cast<int>(SELECT_TYPE2::StageSelect))
			{
				SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::select, SoundProcess::GetSEVolumeEntire(), false);
				selectType2_ = (selectType2_ + (static_cast<int>(SELECT_TYPE2::Max) - 1)) % static_cast<int>(SELECT_TYPE2::Max);
			}
		}
		switch (selectType2_)
		{
		case static_cast<int>(SELECT_TYPE2::StageSelect):
			menuUICtrl_->SetFlag(false);
			if (!moveMapR_ && !moveMapL_)
			{
				if (controller_->Pressed(InputID::Right))
				{
					// PlaySoundMem(se1, DX_PLAYTYPE_BACK);
					if (selectType_ < static_cast<int>(SELECT_TYPE::StageFifteen))
					{
						SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::select, SoundProcess::GetSEVolumeEntire(), false);
						moveMap_ = MoveMax;
						moveMapOld_ = mapPicPosX;
						moveMapR_ = true;
						selectType_ = (selectType_ + 1) % static_cast<int>(SELECT_TYPE::Max);//�I����Ԃ��������
					}
				}

				if (controller_->Pressed(InputID::Left))
				{
					// PlaySoundMem(se1, DX_PLAYTYPE_BACK);
					if (selectType_ > static_cast<int>(SELECT_TYPE::TOne))
					{
						SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::select, SoundProcess::GetSEVolumeEntire(), false);
						moveMap_ = MoveMin;
						moveMapOld_ = mapPicPosX;
						moveMapL_ = true;
						selectType_ = (selectType_ + (static_cast<int>(SELECT_TYPE::Max) - 1)) % static_cast<int>(SELECT_TYPE::Max);			//�I����Ԃ���グ��
					}
				}

				if (controller_->Pressed(InputID::Attack))			// ����L�[�������ꂽ���̏�ԑJ��
				{
					SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::select, SoundProcess::GetSEVolumeEntire(), false);
					SoundProcess::SoundStop(SoundProcess::SOUNDNAME_BGM::select);
					return UpdateNext(std::move(ownScene));
				}
			}
			break;
		case static_cast<int>(SELECT_TYPE2::Other):
			menuUICtrl_->SetFlag(true);
			menuUICtrl_->Update(delta);
			if (controller_->Pressed(InputID::Attack))
			{
				SoundProcess::SoundStop(SoundProcess::SOUNDNAME_BGM::select);
				if (menuUICtrl_->GetNowMenu() == SELECT_TYPE3::Title)
				{
					// �T�E���h�̐ݒ�̎�
					time_ = 0.0f;
					return std::make_unique<TitleScene>();
				}
				else if (menuUICtrl_->GetNowMenu() == SELECT_TYPE3::Setting)
				{
					time_ = 0.0f;
					SoundProcess::SoundStop(SoundProcess::SOUNDNAME_BGM::select);
					return std::make_unique<OptionScene>(std::move(ownScene));
				}
			}
			break;
			default:
			break;
		}
	}
	return ownScene;
}

uniqueScene SelectScene::UpdateNext(uniqueScene ownScene)
{
	mapNum_ = selectType_;
	// �}�b�v���܂�����Ă��Ȃ��Ƃ��̓Q�[�����n�܂�Ȃ��悤��


	if (mapNum_ == 0)
	{
		Relese();
		return std::make_unique<LoadScene>(
			2.0,
			std::move(ownScene),
			std::make_unique<PlayerTutorial>(mapNum_ +1)
			);
	}
	else if (mapNum_ == 1)
	{
		Relese();
		return std::make_unique<LoadScene>(
			2.0,
			std::move(ownScene),
			std::make_unique<ItemTutorial>(mapNum_ + 1)
			);
	}
	else if (mapNum_ == 2)
	{
		Relese();
		return std::make_unique<LoadScene>(
			2.0,
			std::move(ownScene),
			std::make_unique<DoorTutorial>(mapNum_ + 1)
			);
	}
	else
	{
		Relese();
		return std::make_unique<LoadScene>(
			2.0,
			std::move(ownScene),
			std::make_unique<GameScene>(mapNum_ -2, stageData_[mapNum_].size_ == MapSize::Small)
			);
	}
}


void SelectScene::DrawOwnScreen(float delta)
{
	// �������g�̃X�N���[���ɑ΂���Draw
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawBg();
	//DrawTutorial();
	DrawSetting();
}

void SelectScene::DrawBg()
{
	// �w�i�̕`��
	DrawRotaGraph(550, 300, 4.5, 0.0, lpImageMng.GetID("bg")[0], true);
	DrawRotaGraph(far_buildingsPos_, 345, 4.5, 0.0, lpImageMng.GetID("far-buildings")[0], true);
	DrawRotaGraph(far_buildingsPos_ - 1200, 345, 4.5, 0.0, lpImageMng.GetID("far-buildings")[0], true);
	if (far_buildingsPos_ == 1580)	// �X�N���[��
	{
		far_buildingsPos_ = far_buildingsPos_ - 1200;
	}
	far_buildingsPos_++;
	DrawRotaGraph(buildings_, 330, 4.5, 0.0, lpImageMng.GetID("buildings")[0], true);
	DrawRotaGraph(buildings_ - 1200, 330, 4.5, 0.0, lpImageMng.GetID("buildings")[0], true);
	if (buildings_ == 1670)
	{
		buildings_ = buildings_ - 1200;
	}
	buildings_ += 2;
	DrawRotaGraph(skill_foreground_, 430, 4.5, 0.0, lpImageMng.GetID("skill-foreground")[0], true);
	DrawRotaGraph(skill_foreground_ - 1200, 430, 4.5, 0.0, lpImageMng.GetID("skill-foreground")[0], true);
	if (skill_foreground_ == 1660)
	{
		skill_foreground_ = skill_foreground_ - 1200;
	}
	skill_foreground_ += 3;
	DrawGraph(0, 0, lpImageMng.GetID("fogBG")[0], true);
}

void SelectScene::DrawSetting(void)
{
	if (selectType_ == 0)
	{
		DrawMap(lpImageMng.GetID("StageString")[selectType_], false);
	}
	else if (selectType_ == (num - 1))
	{
		DrawMap(lpImageMng.GetID("StageString")[selectType_], true, false);
	}
	else
	{
		DrawMap(lpImageMng.GetID("StageString")[selectType_]);
	}


	menuUICtrl_->Draw();
}


void SelectScene::DrawMap(int StageID, bool Left, bool Right)
{
	

	if (moveMapR_)
	{
		if (mapPicPosX < (moveMap_ - speed))
		{
			moveMapOld_ -= speed;
			moveMap_ -= speed;
		}

		else
		{

			moveMapOld_ = MoveMin;
			moveMap_ = mapPicPosX;
			moveMapR_ = false;
		}
		
		DrawRotaGraph(static_cast<int>(moveMapOld_), static_cast<int>(mapPicPosY), 0.9, 0, gameMap_, true);
		auto tmpIdx = std::clamp(selectType_ - 1, 0, 10);
		DrawRotaGraph(static_cast<int>(moveMapOld_), static_cast<int>(mapPicPosY), 1.0, 0, lpImageMng.GetID("Stage")[stageData_[tmpIdx].stageImgIdx_], true);
	}

	if (moveMapL_)
	{
		
		if (mapPicPosX > (moveMap_ + speed))
		{
		
			moveMapOld_ += speed;
			moveMap_ += speed;
		}
		else
		{
			
			moveMapOld_ = MoveMax;
			moveMap_ = mapPicPosX;
			moveMapL_ = false;
		}
		DrawRotaGraph(static_cast<int>(moveMapOld_), static_cast<int>(mapPicPosY), 0.9, 0, gameMap_, true);
		DrawRotaGraph(static_cast<int>(moveMapOld_), static_cast<int>(mapPicPosY), 1.0, 0, lpImageMng.GetID("Stage")[stageData_[selectType_ + 1].stageImgIdx_], true);
	}
	
	DrawRotaGraph(static_cast<int>(moveMap_), static_cast<int>(mapPicPosY), 0.9, 0, gameMap_, true);

	DrawRotaGraph(static_cast<int>(moveMap_), static_cast<int>(mapPicPosY), 1.0, 0, lpImageMng.GetID("Stage")[stageData_[selectType_].stageImgIdx_], true);
	
	/*if (!moveMapL_ && !moveMapR_)
	{
		DrawFormatString(static_cast<int>(moveMap_), static_cast<int>(mapPicPosY - 100), 0x000000, mapsize.c_str());
	}*/
	if (selectType2_ == 0)
	{
		SetDrawBright(255,128, 0);
	}
	DrawRotaGraph(ScreenSizeX / 2, ScreenSizeY / 2 + 150, 1.0, 0.0, StageID, true);
	SetDrawBright(255, 255, 255);

	// �G�̐��̈ꌅ�ڂƓ񌅖ڂ����o��
	int tmp1 = (stageData_[selectType_].enemyNum_/10)%10;
	int tmp2 = (stageData_[selectType_].enemyNum_ - tmp1 * 10) % 10;
	
	// �G�̐���\��
	DrawRotaGraph(ScreenSizeX / 2 , ScreenSizeY / 2  + 190, 1.0, 0.0, lpImageMng.GetID("EnemyCount")[0], true);
	DrawRotaGraph(ScreenSizeX / 2 + 93, ScreenSizeY / 2 +190 ,1.0,0.0, lpImageMng.GetID("SelectNumber")[tmp1], true);
	DrawRotaGraph(ScreenSizeX / 2 + 113, ScreenSizeY / 2 + 190,1.0,0, lpImageMng.GetID("SelectNumber")[tmp2], true);

	// �Ď��J�����̐��̈ꌅ�ڂƓ񌅂����o��
	tmp1 = (stageData_[selectType_].securityCamNum_ / 10) % 10;
	tmp2 = (stageData_[selectType_].securityCamNum_ - tmp1 * 10) % 10;

	// �Ď��J�����̐���\��
	DrawRotaGraph(ScreenSizeX / 2 , ScreenSizeY / 2 + 210, 1.0, 0.0, lpImageMng.GetID("SecurityCamCount")[0], true);
	DrawRotaGraph(ScreenSizeX / 2 + 93, ScreenSizeY / 2 + 210, 1.0, 0.0, lpImageMng.GetID("SelectNumber")[tmp1], true);
	DrawRotaGraph(ScreenSizeX / 2 + 113, ScreenSizeY / 2 + 210, 1.0, 0, lpImageMng.GetID("SelectNumber")[tmp2], true);

	// �}�b�v�T�C�Y�\��
	int sizeNum{ 0 };
	if (stageData_[selectType_].size_ != MapSize::Big)
	{
		sizeNum = 1;
	}
	DrawRotaGraph(ScreenSizeX / 2, ScreenSizeY / 2 + 230, 1.0, 0.0, lpImageMng.GetID("MapSizeString")[0], true);
	DrawRotaGraph(ScreenSizeX / 2 + 103, ScreenSizeY / 2 + 230, 1.0, 0.0, lpImageMng.GetID("MapSize")[sizeNum], true);

	if (Left)
	{
		// MyDrawRotaGraph((ScreenSizeX / 2) + (StringPos - (ScreenSizeX / 2 + selecterLPos_)), (ScreenSizeY / 2) + (OffsetY - (ScreenSizeY / 2 - 10)), 0.1, 0, selecterLPic_);
		
		DrawRotaGraph(static_cast<int>((ScreenSizeX / 2.0f) + (StringPos - (ScreenSizeX / 2.0f + selecterLPos))), static_cast<int>((ScreenSizeY / 2) + (OffsetY - (ScreenSizeY / 2 - 10))), 0.1, 0, selecterLPic_, true);
	}
	if ( Right)
	{
		
		DrawRotaGraph((ScreenSizeX / 2) - (static_cast<int>(StringPos) - (ScreenSizeX / 2 + selecterLPos)), (static_cast<int>(ScreenSizeY) / 2) + (OffsetY - (ScreenSizeY / 2 - 10)), 0.1, 0, selecterPic_, true);
	}
}


constexpr int version = 1;
constexpr char s[4]{ 's','d','a', 't' };
struct Header 
{
	char sig[4] = { 's','d','a', 't' };
	int ver = version;
	unsigned int sum = 0;
};


// �X�e�[�W�̃f�[�^�t�@�C����ǂݍ���
bool SelectScene::LoadStageData(void)
{
	// �t�@�C�����J��
	std::ifstream file("Resource/StageData/StageData.sdat");

	// �J���Ă��邩�`�F�b�N
	if (!file)
	{
		DebugLog("�t�@�C�����J���܂���ł���");
		return false;
	}

	// �w�b�_�[�����擾
	Header h;
	file.read(reinterpret_cast<char*>(&h), sizeof(h));

	// �V�O�l�`�����`�F�b�N
	for (int i = 0; i < 4; i++)
	{
		if (h.sig[i] != s[i])
		{
			return false;
		}
	}

	// �o�[�W�����`�F�b�N
	if (h.ver != version)
	{
		DebugLog("�Ή����Ă��Ȃ��o�[�W�����ł�");
		return false;
	}

	// �f�[�^�̓ǂݍ���
	int size = 0;
	file.read(reinterpret_cast<char*>(&size), sizeof(size));
	stageData_.resize(size);
	file.read(reinterpret_cast<char*>(stageData_.data()), sizeof(stageData_[0]) * stageData_.size());


	// �f�[�^���`�F�b�N
	unsigned int sum{0};
	for (auto& data : stageData_)
	{
		sum += static_cast<unsigned int>(data.enemyNum_);
		sum += static_cast<unsigned int>(data.securityCamNum_);
		sum += static_cast<unsigned int>(data.stageImgIdx_);
		sum += static_cast<unsigned int>(data.size_);
	}

	if (sum != h.sum)
	{
		DebugLog("�l������������܂���");
		return false;
	}

	return true;
}

void SelectScene::Relese(void)
{
	lpImageMng.Relese("Stage");
	lpImageMng.Relese("bg");
	lpImageMng.Relese("buildings");
	lpImageMng.Relese("far-buildings");
	lpImageMng.Relese("skill-foreground");
	lpImageMng.Relese("fog");
	lpImageMng.Relese("fogBG");
	lpImageMng.Relese("Resource/Image/SelectScene/selecter.png");
	lpImageMng.Relese("Resource/Image/SelectScene/selecter_L.png");
	lpImageMng.Relese("Resource/Image/SelectScene/gameMap.png");

	lpImageMng.Relese("EnemyCount");
	lpImageMng.Relese("SecurityCamCount");
	lpImageMng.Relese("SelectNumber");
	lpImageMng.Relese("StageString");
	lpImageMng.Relese("MapSize");
	lpImageMng.Relese("MapSizeString");
}


