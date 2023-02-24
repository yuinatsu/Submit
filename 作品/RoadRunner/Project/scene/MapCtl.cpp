#include <DxLib.h>
#include <algorithm>			// clamp���g�p����̂ɕK�v
#include <fstream>
#include <iostream>
#include "MapCtl.h"
#include "SceneMng.h"
#include "DrawChip.h"
#include "Obj/Player.h"
#include "Obj/Enemy.h"
#include "../common/ImageMng.h"
#include "../_debug/_DebugConOut.h"


MapCtl* MapCtl::s_Instance = nullptr;


void MapCtl::Create(SceneID sceneID)
{
	if (s_Instance == 0)
	{
		s_Instance = new MapCtl(sceneID);
	}
}


void MapCtl::Destroy(void)
{
	if (s_Instance)
	{
		delete s_Instance;
	}
	s_Instance = nullptr;
}


Vector2 MapCtl::GetDrawOffset()
{
	if (!camera_.expired())				// �J�����������Ă���΃J�����̃I�t�Z�b�g�{�`��p�I�t�Z�b�g��Ԃ�
	{
		return camera_.lock()->GetDrawOffset() + Vector2{ lpSceneMng.drawOffset_.x ,0 };
	}
	// �J����������ł���Ε`��p�I�t�Z�b�g��Ԃ�
	return lpSceneMng.drawOffset_;
}

bool MapCtl::CheckMapChar(ChipID id)
{
	//Vector2 ckPos(0, 0);
	//if (id == ChipID::PL)						// ���@�̎g�p����
	//{
	//	for (ckPos.y = 0; ckPos.y < worldArea_.y; ckPos.y++)
	//	{
	//		for (ckPos.x = 0; ckPos.x < worldArea_.x; ckPos.x++)
	//		{
	//			if (GetMapDataEdit(ChipGP::CHAR, mapID_[ckPos.y][ckPos.x]) == ChipID::PL)
	//			{
	//				mapID_[ckPos.y][ckPos.x] = 0;
	//				return false;
	//			}
	//		}
	//	}
	//}
	//if (id == ChipID::ENEMY)						// �G�̎g�p����
	//{
	//	for (ckPos.y = 0; ckPos.y < worldArea_.y; ckPos.y++)
	//	{
	//		for (ckPos.x = 0; ckPos.x < worldArea_.x; ckPos.x++)
	//		{
	//			if (GetMapDataEdit(ChipGP::CHAR, mapID_[ckPos.y][ckPos.x]) == ChipID::ENEMY)
	//			{
	//				mapID_[ckPos.y][ckPos.x] = 0;
	//				return false;
	//			}
	//		}
	//	}
	//}
	return false;
}


void MapCtl::EditDraw(void)
{
	DrawMap();
	DrawEditLine();
	//animCnt_++;			// �G�f�B�b�g�����A�j���[�V���������������Ȃ�L�q
}


void MapCtl::GameDraw(void)
{
	DrawMap();
	animCnt_++;				// �G�f�B�b�g�ŃA�j���[�V�������������Ȃ�����Q�[���ł̂݃J�E���g�A�b�v������
}


void MapCtl::DrawEditOffset()
{
	auto gameScreenSize = viewArea_ * chipSize_;					// �Q�[���X�N���[���̃T�C�Y
	DrawTriangle(drawOffset_.x / 3, (gameScreenSize.y / 2) + drawOffset_.y, (drawOffset_.x * 2) / 3, (gameScreenSize.y / 2) + drawOffset_.y + chipSize_.y, (drawOffset_.x * 2) / 3, (gameScreenSize.y / 2) + drawOffset_.y - chipSize_.y, CrL_, true);			// �����
	DrawTriangle(gameScreenSize.x + drawOffset_.x * 5 / 3, (gameScreenSize.y / 2) + drawOffset_.y, gameScreenSize.x + (drawOffset_.x * 4) / 3, (gameScreenSize.y / 2) + drawOffset_.y + chipSize_.y, gameScreenSize.x + (drawOffset_.x * 4) / 3, (gameScreenSize.y / 2) + drawOffset_.y - chipSize_.y, CrR_, true);			// �E���
}


bool MapCtl::SetMapData(Vector2 pos, ChipID id)
{
	return SetMapData(pos, id, chipGP_[id]);
}


bool MapCtl::SetMapData(Vector2 pos, ChipID id, ChipGP gp)
{
	Vector2 tmpPos(pos / chipSize_);				// �����t���R���X�g���N�^�ō��Wpos���}�X�ڐ�tmpPos�ɕϊ�����

	// �ʉ�
	//-------
	//Vector2 tmpPos;									// ���Wpos���}�X�ڐ�tmpPos�ɕϊ�����
	//tmpPos.x = pos.x / chipSize_.x;					// �J�[�\����X���W�̃}�X�ڈʒu
	//tmpPos.y = pos.y / chipSize_.y;					// �J�[�\����Y���W�̃}�X�ڈʒu

	if (CheckMapChar(id))					// �L�����̎g�p�����`�F�b�N
	{
		return false;
	}

	if ((tmpPos.x <= worldArea_.x) && (tmpPos.x >= 0) && (tmpPos.y <= worldArea_.y) && (tmpPos.y >= 0))			// �ϊ���̒l���z��͈͓̔����`�F�b�N����
	{
		if ((id == ChipID::BLANK) && (gp == ChipGP::MAX))
		{
			mapID_[tmpPos.y][tmpPos.x] = 0;
			return true;
		}
		auto defBit = mapID_[tmpPos.y][tmpPos.x] & (~(0x000000ff << (static_cast<int>(gp) * 8)));				// ����Ȃ��r�b�g�����
		auto getBit = static_cast<int>(id) << (static_cast<int>(gp) * 8);								// �K�v�ȃr�b�g��id�ɑ��
		mapID_[tmpPos.y][tmpPos.x] = defBit | getBit;				// �l��z��ɑ������

		//mapID_[tmpPos.y][tmpPos.x] = static_cast<int>(id);
		return true;
	}

	return false;
}


ChipID MapCtl::GetMapDataEdit(ChipGP gpID, unsigned int data)
{
	unsigned int shiftData = data >> (static_cast<unsigned int>(gpID) * 8);					// baseMapID��data��bit��gpID(�o�C�g���r�b�g�ɕϊ�)���r�b�g�V�t�g
	return static_cast<ChipID>(shiftData & 0x000000ff);										// �r�b�g�V�t�g�������ʂ̍Ō���݂̂��A���h�Ŏc���Areturn����
}


ChipID MapCtl::GetMapDataGame(ChipGP gpID, unsigned int data)
{
	unsigned int shiftData = data >> (static_cast<unsigned int>(gpID) * 8);
	auto id = static_cast<ChipID>(shiftData & 0x000000ff);
	if (id == ChipID::ESC)					// �E�o�p�͂������𔻒f
	{
		if (!GetGoldCnt())					// ��������ׂĎ擾���Ă��邩�𔻒f
		{
			id = ChipID::LAD;				// �E�o�͂������͂����Ƃ��Ĉ���
		}
		else
		{
			id = ChipID::BLANK;
		}
	}
	return id;
}


ChipID MapCtl::GetMapData(ChipGP gpID, int x, int y, bool dig)				// x, y �Ɋւ��Ă͌�����v�f���𒴂���\��������̂Ŕ͈̓`�F�b�N�����Ă���
{
	x = std::clamp(x, 0, (worldArea_.x - 1));						// x���W�̃}�X�ڂ͈̔̓`�F�b�N�ix���͈͂𒴂��Ă�����ݒ肵���ő�l�ŏ��l��x�ɕ␳�j
	y = std::clamp(y, 0, (worldArea_.y - 1));						// y���W�̃}�X�ڂ͈̔̓`�F�b�N
	if ((dig) && (mapDig_[y][x]))
	{
		return ChipID::BLANK;										// �����@���Ă�����BLANK�ɂ���
	}

	//return GetMapDataEdit(gpID, mapID_[y][x]);							// �}�X�ڂ���f�[�^���擾
	return GetMapDataFunc(gpID, mapID_[y][x]);								// Func��GAME��EDIT�𕪊�
}


ChipID MapCtl::GetMapData(ChipGP gpID, Vector2 pos, bool dig = true)
{
	Vector2 mPos(pos / chipSize_);					// ���Wpos���}�X�ڐ�tmpPos�ɕϊ�����

	return GetMapData(gpID, mPos.x, mPos.y, dig);		// �}�X�ڂ�͈̓`�F�b�N���ɍs��
}


bool MapCtl::SetDig(const Vector2& pos, DIR_LR dir)
{
	Vector2 tmpPos((pos + Vector2(dir == DIR_LR::LEFT ?  -chipSize_.x : chipSize_.x, chipSize_.y)) / chipSize_);
	if ((tmpPos.x < 0) || (tmpPos.x >= worldArea_.x) || (tmpPos.x < 0) || (tmpPos.y >= worldArea_.y))				// �͈͊O�`�F�b�N
	{
		return false;
	}
	if (GetMapData(ChipGP::BG, tmpPos.x, tmpPos.y, true) != ChipID::BBL)					// �󂹂�u���b�N�ȊO�͌@��Ȃ�
	{
		return false;
	}
	auto ckData = GetMapData(ChipGP::BG, tmpPos.x, tmpPos.y - 1, true);
	if ((ckData == ChipID::BBL) || (ckData == ChipID::BL) || (ckData == ChipID::LAD) || (ckData == ChipID::TRAP))
	{
		return false;
	}

	mapDig_[tmpPos.y][tmpPos.x] = DigFullFrame;			// �@����s
	digPosList_.push_back(tmpPos);
	return true;
}


int MapCtl::GetDigData(int x, int y)
{
	if ((x < 0) || (x >= worldArea_.x) || (y < 0) || (y >= worldArea_.y))				// �͈͊O�`�F�b�N
	{
		return 0;
	}
	return mapDig_[y][x];
}


void MapCtl::DigUpdate(void)
{
	for (auto& pos : digPosList_)
	{
		mapDig_[pos.y][pos.x]--;
	}

	digPosList_.remove_if([&](Vector2 vec) {				// true�Ȃ�mapDig_[][]�̒l���폜		�����_�� ... []() {}; -> [] ... �L���v�`���[���A() ... �������A{} ... ���̕�(�����̒��g)		�^���_(auto)�ŋL�q���y		remove_if�͈������P�K�v
		if (mapDig_[vec.y][vec.x] > 0)						// mapDig_�̒l���O���傫�����remove���Ȃ��Ă����̂�false
		{
			return false;
		}
		return true;
		}
	);														// remove_if�̃J�b�R�� ... �����֐�

	// �ʉ��P(�ϐ��Ɉ�x�����)
	//----------------------------
	//auto prg = [&](Vector2 vec) {				// �����_�� ... []() {}; -> [] ... �L���v�`���[���A() ... �������A{} ... ���̕�(�����̒��g)		�^���_(auto)�ŋL�q���y		remove_if�͈������P�K�v
	//	if (mapDig_[vec.y][vec.x] > 0)			// mapDig_�̒l���O���傫�����remove���Ȃ��Ă����̂�false
	//	{
	//		return false;
	//	}
	//	return true;
	//};
	//digPosList_.remove_if(prg);
	// �ʉ��Q(mapDig_[pos.y][pos.x]--���܂߂�remove_if)
	//----------------------------------------------------
	//digPosList_.remove_if([&](Vector2 vec) {
	//	return !(--mapDig_[vec.y][vec.x]);
	//	}
	//);
}


bool MapCtl::EraceGold(Vector2 pos, bool playerFlag)
{
	if (GetMapData(ChipGP::ITEM, pos, false) == ChipID::GOLD)					// ���̃}�X�ɋ��򂪂��邩�𔻒f
	{
		if (playerFlag)													// ���@��������擾���������̎c�萔�����炷
		{
			goldCnt_--;
		}
		SetMapData(pos, ChipID::BLANK, ChipGP::ITEM);			// ���򂪂������ꏊ��BLANK�ɍ����ւ���
	}
	return false;
}


void MapCtl::InputButtonL(bool input)
{
	if (input)					// �����ꂽ��g�F�h
	{
		CrL_ = 0x0000ff;
	}
	if (!input)					// ������Ԃ́g���F�h
	{
		CrL_ = 0xffff00;
	}
}


void MapCtl::InputButtonR(bool input)
{
	if (input)					// �����ꂽ��g�F�h
	{
		CrR_ = 0x0000ff;
	}
	if (!input)					// ������Ԃ́g���F�h
	{
		CrR_ = 0xffff00;
	}
}


void MapCtl::SetCamera(std::weak_ptr<Camera> camera)
{
	camera_ = camera;
}


int MapCtl::GetGoldCnt()
{
	return goldCnt_;
}


bool MapCtl::MapSave(void)
{
	DataHeader expData = {
		LDR_ID_NAME,
		LDR_ID_VER,
		{0,0},
		worldArea_.x,
		worldArea_.y,
		{0,0,0},
		0x00				// 0
	};

	for (auto num : baseMapID_)
	{
		expData.sum += static_cast<unsigned char>(num);
	}

	//
	std::ofstream ofp("mapdata.txt", std::ios::binary);			// �������݃e�L�X�g
	ofp.write(reinterpret_cast<char*>(&expData), sizeof(expData));
	ofp.write(reinterpret_cast<char*>(baseMapID_.data()), baseMapID_.size() * sizeof(baseMapID_[0]));
	// �ʉ�
	//-------
	//FILE* file;
	//fopen_s(&file, "data/mapdata.map", "wb");			// file = fopen("data/mapdata.map", "wb");
	//fwrite(&expData, sizeof(DataHeader), 1, file);
	//fwrite(&baseMapID_[0], baseMapID_.size() * sizeof(unsigned int), 1, file);
	//fclose(file);

	return true;
}


bool MapCtl::MapLoad(std::string fileName)
{
	InitMap();			// ��Ƀf�[�^���N���A����

	DataHeader expData;

	std::ifstream ifp(fileName, std::ios::binary);			// �ǂݏo���e�L�X�g
	ifp.read(reinterpret_cast<char*>(&expData), sizeof(expData));
	ifp.read(reinterpret_cast<char*>(baseMapID_.data()), baseMapID_.size() * sizeof(baseMapID_[0]));

	bool headerFlag = true;
	if (std::string(expData.fileID) != LDR_ID_NAME)			// ID�̊m�F����
	{
		headerFlag = false;
		TRACE("�f�[�^������܂���\n");
	}

	if (expData.verID != LDR_ID_VER)				// �o�[�W�����̊m�F����
	{
		headerFlag = false;
		TRACE("�o�[�W�������Ⴂ�܂�\n");
	}

	unsigned char sum = 0;
	for (auto num : baseMapID_)
	{
		sum += static_cast<unsigned char>(num);
		if (GetMapDataEdit(ChipGP::ITEM, num) == ChipID::GOLD)		// �}�b�v���̋���̐����v��
		{
			goldCnt_++;
		}
	}

	if (expData.sum != sum)						// sum�l�̊m�F����
	{
		headerFlag = false;
		TRACE("sum�l���ُ�ł�\n");
	}

	if (!headerFlag)				// ID�A�o�[�W�����ԍ��ASUM�l�����ꂩ���������ǂݍ��܂�Ă��Ȃ�������false�����^�[��
	{
		InitMap();					// �f�[�^���N���A���Ă���
		return headerFlag;
	}

	// �܂Ƃ߂Ċm�F��������ꍇ
	//---------------------------
	//unsigned char sum = 0;
	//for (auto num : baseMapID_)
	//{
	//	sum += static_cast<unsigned char>(num);
	//}

	//if ((LDR_ID_NAME) && (expData.verID == LDR_ID_VER) && (expData.sum == sum))				// ID�A�o�[�W�����ԍ��ASUM�l���������ǂݍ��܂�Ă��邩�m�F
	//{
	//	InitMap();					// �f�[�^���N���A���Ă���
	//	return false;
	//}

	return headerFlag;
}


bool MapCtl::MapLoad(std::string fileName, BaseScene& scene)
{
	if (!MapLoad(fileName))
	{
		return false;
	}

	for (int y = 0; y < worldArea_.y; y++)
	{
		for (int x = 0; x < worldArea_.x; x++)
		{
			// Player
			//---------
			if (GetMapData(ChipGP::CHAR, x, y, false) == ChipID::PL)
			{
				auto itr = scene.AddObjList(std::make_shared<Player>(scene.GetSceneID(), camera_));
				(*itr)->SetPos(Vector2 (x, y) * chipSize_);							// ���ݍ��W�Z�b�g			ObjList -> ObjList::iterator -> std::shared_ptr<Obj> -> Obj -> SetPos()
				(*itr)->SetDefPos(Vector2(x, y) * chipSize_);						// �f�t�H���g���W�Z�b�g

				if (!camera_.expired())											// camera_��weak_ptr�̐�(shared_ptr)���L�����ǂ������f
				{
					camera_.lock()->SetTarget(*itr);
				}
			}

			// Enemy
			//--------
			if (GetMapData(ChipGP::CHAR, x, y, false) == ChipID::ENEMY)
			{
				auto itr = scene.AddObjList(std::make_shared<Enemy>(scene.GetSceneID(), camera_));
				(*itr)->SetPos(Vector2(x, y) * chipSize_);							// ���ݍ��W�Z�b�g			ObjList -> ObjList::iterator -> std::shared_ptr<Obj> -> Obj -> SetPos()
				(*itr)->SetDefPos(Vector2(x, y) * chipSize_);						// �f�t�H���g���W�Z�b�g
			}
		}
	}
	return true;
}


bool MapCtl::MapDel(void)
{
	for (auto& data : baseMapID_)
	{
		data = 0;
	}

	return true;
}


void MapCtl::DrawMap(void)
{
	// �͂��� or ���� �̃A�j���[�V����
	//----------------------------------
	auto ConvID = [&](ChipID id) {
		if ((id == ChipID::LAD) || (id == ChipID::GOLD))
		{
			return (static_cast<unsigned int>(id) + abs(((animCnt_ / 40) % 4) - 2) * (static_cast<unsigned int>(ChipID::MAX)));				// (/ 40) ... �t���[���Ԃ̎���, (% 4) ... �A�j���[�V�����̃R�}�� - 1, (- 2) ... 2.1.0.1.2.1.0...�ƃR�}���i��, (* 11) ... tileset��X���W
		}
		return static_cast<unsigned int>(id);
	};


	// �}�b�v�`�b�v�`��
	//-------------------
	Vector2 camOffset;			// �`��p�I�t�Z�b�g�̕ϐ�
	if (!camera_.expired())
	{
		// �J�����������Ă���Ε`��p�I�t�Z�b�g���Z�b�g����
		camOffset = camera_.lock()->GetDrawOffset();
	}
	int chipOffsetX = -(camOffset.x / chipSize_.x);
	for (auto chipGP : ChipGP())				// ChipGP��͈�for���ŉ�
	{
		for (int y = 0; y < worldArea_.y; y++)					// 
		{
			for (int x = chipOffsetX; x <= viewArea_.x + chipOffsetX; x++)					// �}�X�ڍ��W�n
			{
				auto id = GetMapData(chipGP, x, y, false);				// GetMapData���Ăяo��				true�ɂ����Dig�̃A�j���[�V�������o�Ȃ�
				DrawChip_[id](x, y, chipSize_, camOffset, lpImageMng.GetID("tileset")[ConvID(id)]);
				//DrawChipDef()(x, y, chipSize_, camOffset, lpImageMng.GetID("tileset")[id]);
				//DrawGraph(x * chipSize_.x + camOffset.x, y * chipSize_.y + camOffset.y, lpImageMng.GetID("tileset")[id], true);					// ��ʂɃZ�b�g����chipID�̕\��
			}
		}
	}
}


void MapCtl::DrawEditLine(void)
{
	for (int y = 0; y <= viewArea_.y; y++)							// �ҏW��ʗp�����C���i�����C�����c�ɐL�΂��j
	{
		DrawLine(0, chipSize_.y * y, viewArea_.x * chipSize_.x, chipSize_.y * y, 0xffffff);
	}
	for (int x = 0; x <= viewArea_.x; x++)							// �ҏW��ʗp�c���C���i�c���C�������ɐL�΂��j
	{
		DrawLine(chipSize_.x * x, 0, chipSize_.x * x, viewArea_.y * chipSize_.y, 0xffffff);
	}
}


MapCtl::MapCtl(SceneID sceneID)
{
	sceneID_ = sceneID;					// sceneID��ۑ�
	Init();

	if (sceneID_ == SceneID::EDIT)
	{
		DrawFunc = std::bind(&MapCtl::EditDraw, this);					// EditScene�ŁA�}�b�v�Ɛ���`��
		GetMapDataFunc = std::bind(&MapCtl::GetMapDataEdit, this, std::placeholders::_1, std::placeholders::_2);				// std::placeholders::_1 ... GetMapDataEdit��()�̒��g�̈�Ԗڂ̕ϐ�
		InitDrawChipEdit();
	}
	else
	{
		DrawFunc = std::bind(&MapCtl::GameDraw, this);					// GameScene�ŁA�}�b�v�݂̂�`��
		GetMapDataFunc = std::bind(&MapCtl::GetMapDataGame, this, std::placeholders::_1, std::placeholders::_2);
		InitDrawChipGame();
	}
}


MapCtl::~MapCtl()
{
}


bool MapCtl::Init(void)
{
	lpImageMng.GetID("image/tileset.png", "tileset", { 32, 32 }, { 11,6 });
	
	animCnt_ = 0;

	viewArea_   = lpSceneMng.viewArea_;
	worldArea_  = lpSceneMng.worldArea_;
	chipSize_   = lpSceneMng.chipSize_;
	drawOffset_ = lpSceneMng.drawOffset_;


	chipGP_[ChipID::BLANK]    = ChipGP::MAX;			// ��
	chipGP_[ChipID::BBL]      = ChipGP::BG;				// �@���u���b�N
	chipGP_[ChipID::BL]       = ChipGP::BG;				// �@��Ȃ��u���b�N
	chipGP_[ChipID::LAD]      = ChipGP::BG;				// �͂���
	chipGP_[ChipID::BAR]      = ChipGP::BG;				// �o�[
	chipGP_[ChipID::TRAP]     = ChipGP::BG;				// �g���b�v
	chipGP_[ChipID::ESC]      = ChipGP::BG;				// �E�o�͂���
	chipGP_[ChipID::GOLD]     = ChipGP::ITEM;			// �S�[���h
	chipGP_[ChipID::ENEMY]    = ChipGP::CHAR;			// �G
	chipGP_[ChipID::PL]       = ChipGP::CHAR;			// ���@
	chipGP_[ChipID::EDIT_CUR] = ChipGP::CHAR;			// edit�p�J�[�\��


	InitMap();

	//EditDraw();

	return true;
}


bool MapCtl::InitMap(void)
{
	baseMapID_.resize(worldArea_.x * worldArea_.y);			// �}�b�v�̑��T�C�Y��		e�c����...y
	mapID_.resize(worldArea_.y);							// ������...x

	for (int y = 0; y < worldArea_.y; y++)					// for (int y = 0; y < mapID_.size(); y++) �ł���
	{
		mapID_[y] = &baseMapID_[worldArea_.x * y];			// mapID_��baseMapID_�̃|�C���^���i�[
	}

	for (auto& data : baseMapID_)							// �Q�ƂŃf�[�^����������
	{
		data = 0;
	}

	InitDig();
	goldCnt_ = 0;

	return true;
}


bool MapCtl::InitDig(void)
{
	baseMapDig_.resize(worldArea_.x * worldArea_.y);		// �}�b�v�̑��T�C�Y��		�c����...y
	mapDig_.resize(worldArea_.y);							// ������...x

	for (int y = 0; y < worldArea_.y; y++)					// for (int y = 0; y < mapID_.size(); y++) �ł���
	{
		mapDig_[y] = &baseMapDig_[worldArea_.x * y];			// mapID_��baseMapID_�̃|�C���^���i�[
	}

	for (auto& data : baseMapDig_)							// �Q�ƂŃf�[�^����������
	{
		data = 0;
	}

	return true;
}


void MapCtl::InitDrawChipEdit()
{
	DrawChip_.try_emplace(ChipID::BLANK,    DrawChipNon());				// ��				// try_emplase ... ��񂪓����Ă�Ή������Ȃ�����񂪓����Ă��Ȃ���Ώ�������
	DrawChip_.try_emplace(ChipID::BBL,      DrawChipDef());				// �@���u���b�N
	DrawChip_.try_emplace(ChipID::BL,       DrawChipDef());				// �@��Ȃ��u���b�N
	DrawChip_.try_emplace(ChipID::LAD,      DrawChipDef());				// �͂���
	DrawChip_.try_emplace(ChipID::BAR,      DrawChipDef());				// �o�[�i�_�j
	DrawChip_.try_emplace(ChipID::TRAP,     DrawChipDef());				// �g���b�v�i���Ƃ����j
	DrawChip_.try_emplace(ChipID::ESC,      DrawChipDef());				// �E�o�͂���
	DrawChip_.try_emplace(ChipID::GOLD,     DrawChipDef());				// �S�[���h�i����j
	DrawChip_.try_emplace(ChipID::ENEMY,    DrawChipDef());				// �G
	DrawChip_.try_emplace(ChipID::PL,       DrawChipDef());				// ���@
	DrawChip_.try_emplace(ChipID::EDIT_CUR, DrawChipNon());				// edit�p�J�[�\��
}


void MapCtl::InitDrawChipGame()
{
	DrawChip_.try_emplace(ChipID::BLANK,    DrawChipNon());				// ��
	DrawChip_.try_emplace(ChipID::BBL,      DrawChipBbl());				// �@���u���b�N
	DrawChip_.try_emplace(ChipID::BL,       DrawChipDef());				// �@��Ȃ��u���b�N
	DrawChip_.try_emplace(ChipID::LAD,      DrawChipDef());				// �͂���
	DrawChip_.try_emplace(ChipID::BAR,      DrawChipDef());				// �o�[�i�_�j
	DrawChip_.try_emplace(ChipID::TRAP,     DrawChipTrap());			// �g���b�v�i���Ƃ����j
	DrawChip_.try_emplace(ChipID::ESC,      DrawChipDef());				// �E�o�͂���
	DrawChip_.try_emplace(ChipID::GOLD,     DrawChipDef());				// �S�[���h�i����j
	DrawChip_.try_emplace(ChipID::ENEMY,    DrawChipNon());				// �G
	DrawChip_.try_emplace(ChipID::PL,       DrawChipNon());				// ���@
	DrawChip_.try_emplace(ChipID::EDIT_CUR, DrawChipNon());				// edit�p�J�[�\��
}
