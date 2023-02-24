#include <DxLib.h>
#include <algorithm>			// clampを使用するのに必要
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
	if (!camera_.expired())				// カメラが生きていればカメラのオフセット＋描画用オフセットを返す
	{
		return camera_.lock()->GetDrawOffset() + Vector2{ lpSceneMng.drawOffset_.x ,0 };
	}
	// カメラが死んでいれば描画用オフセットを返す
	return lpSceneMng.drawOffset_;
}

bool MapCtl::CheckMapChar(ChipID id)
{
	//Vector2 ckPos(0, 0);
	//if (id == ChipID::PL)						// 自機の使用制限
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
	//if (id == ChipID::ENEMY)						// 敵の使用制限
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
	//animCnt_++;			// エディット中もアニメーションをさせたいなら記述
}


void MapCtl::GameDraw(void)
{
	DrawMap();
	animCnt_++;				// エディットでアニメーションさせたくないからゲームでのみカウントアップさせる
}


void MapCtl::DrawEditOffset()
{
	auto gameScreenSize = viewArea_ * chipSize_;					// ゲームスクリーンのサイズ
	DrawTriangle(drawOffset_.x / 3, (gameScreenSize.y / 2) + drawOffset_.y, (drawOffset_.x * 2) / 3, (gameScreenSize.y / 2) + drawOffset_.y + chipSize_.y, (drawOffset_.x * 2) / 3, (gameScreenSize.y / 2) + drawOffset_.y - chipSize_.y, CrL_, true);			// 左矢印
	DrawTriangle(gameScreenSize.x + drawOffset_.x * 5 / 3, (gameScreenSize.y / 2) + drawOffset_.y, gameScreenSize.x + (drawOffset_.x * 4) / 3, (gameScreenSize.y / 2) + drawOffset_.y + chipSize_.y, gameScreenSize.x + (drawOffset_.x * 4) / 3, (gameScreenSize.y / 2) + drawOffset_.y - chipSize_.y, CrR_, true);			// 右矢印
}


bool MapCtl::SetMapData(Vector2 pos, ChipID id)
{
	return SetMapData(pos, id, chipGP_[id]);
}


bool MapCtl::SetMapData(Vector2 pos, ChipID id, ChipGP gp)
{
	Vector2 tmpPos(pos / chipSize_);				// 引数付きコンストラクタで座標posをマス目数tmpPosに変換する

	// 別解
	//-------
	//Vector2 tmpPos;									// 座標posをマス目数tmpPosに変換する
	//tmpPos.x = pos.x / chipSize_.x;					// カーソルのX座標のマス目位置
	//tmpPos.y = pos.y / chipSize_.y;					// カーソルのY座標のマス目位置

	if (CheckMapChar(id))					// キャラの使用制限チェック
	{
		return false;
	}

	if ((tmpPos.x <= worldArea_.x) && (tmpPos.x >= 0) && (tmpPos.y <= worldArea_.y) && (tmpPos.y >= 0))			// 変換後の値が配列の範囲内かチェックする
	{
		if ((id == ChipID::BLANK) && (gp == ChipGP::MAX))
		{
			mapID_[tmpPos.y][tmpPos.x] = 0;
			return true;
		}
		auto defBit = mapID_[tmpPos.y][tmpPos.x] & (~(0x000000ff << (static_cast<int>(gp) * 8)));				// いらないビットを削る
		auto getBit = static_cast<int>(id) << (static_cast<int>(gp) * 8);								// 必要なビットをidに代入
		mapID_[tmpPos.y][tmpPos.x] = defBit | getBit;				// 値を配列に代入する

		//mapID_[tmpPos.y][tmpPos.x] = static_cast<int>(id);
		return true;
	}

	return false;
}


ChipID MapCtl::GetMapDataEdit(ChipGP gpID, unsigned int data)
{
	unsigned int shiftData = data >> (static_cast<unsigned int>(gpID) * 8);					// baseMapIDのdataのbitをgpID(バイトをビットに変換)分ビットシフト
	return static_cast<ChipID>(shiftData & 0x000000ff);										// ビットシフトした結果の最後尾のみをアンドで残し、returnする
}


ChipID MapCtl::GetMapDataGame(ChipGP gpID, unsigned int data)
{
	unsigned int shiftData = data >> (static_cast<unsigned int>(gpID) * 8);
	auto id = static_cast<ChipID>(shiftData & 0x000000ff);
	if (id == ChipID::ESC)					// 脱出用はしごかを判断
	{
		if (!GetGoldCnt())					// 金塊をすべて取得しているかを判断
		{
			id = ChipID::LAD;				// 脱出はしごをはしごとして扱う
		}
		else
		{
			id = ChipID::BLANK;
		}
	}
	return id;
}


ChipID MapCtl::GetMapData(ChipGP gpID, int x, int y, bool dig)				// x, y に関しては元ある要素数を超える可能性があるので範囲チェックをしておく
{
	x = std::clamp(x, 0, (worldArea_.x - 1));						// x座標のマス目の範囲チェック（xが範囲を超えていたら設定した最大値最小値をxに補正）
	y = std::clamp(y, 0, (worldArea_.y - 1));						// y座標のマス目の範囲チェック
	if ((dig) && (mapDig_[y][x]))
	{
		return ChipID::BLANK;										// 穴を掘っていたらBLANKにする
	}

	//return GetMapDataEdit(gpID, mapID_[y][x]);							// マス目からデータを取得
	return GetMapDataFunc(gpID, mapID_[y][x]);								// FuncでGAMEとEDITを分岐
}


ChipID MapCtl::GetMapData(ChipGP gpID, Vector2 pos, bool dig = true)
{
	Vector2 mPos(pos / chipSize_);					// 座標posをマス目数tmpPosに変換する

	return GetMapData(gpID, mPos.x, mPos.y, dig);		// マス目を範囲チェックしに行く
}


bool MapCtl::SetDig(const Vector2& pos, DIR_LR dir)
{
	Vector2 tmpPos((pos + Vector2(dir == DIR_LR::LEFT ?  -chipSize_.x : chipSize_.x, chipSize_.y)) / chipSize_);
	if ((tmpPos.x < 0) || (tmpPos.x >= worldArea_.x) || (tmpPos.x < 0) || (tmpPos.y >= worldArea_.y))				// 範囲外チェック
	{
		return false;
	}
	if (GetMapData(ChipGP::BG, tmpPos.x, tmpPos.y, true) != ChipID::BBL)					// 壊せるブロック以外は掘れない
	{
		return false;
	}
	auto ckData = GetMapData(ChipGP::BG, tmpPos.x, tmpPos.y - 1, true);
	if ((ckData == ChipID::BBL) || (ckData == ChipID::BL) || (ckData == ChipID::LAD) || (ckData == ChipID::TRAP))
	{
		return false;
	}

	mapDig_[tmpPos.y][tmpPos.x] = DigFullFrame;			// 掘り実行
	digPosList_.push_back(tmpPos);
	return true;
}


int MapCtl::GetDigData(int x, int y)
{
	if ((x < 0) || (x >= worldArea_.x) || (y < 0) || (y >= worldArea_.y))				// 範囲外チェック
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

	digPosList_.remove_if([&](Vector2 vec) {				// trueならmapDig_[][]の値を削除		ラムダ式 ... []() {}; -> [] ... キャプチャー部、() ... 引数部、{} ... 実体部(処理の中身)		型推論(auto)で記述が楽		remove_ifは引数が１つ必要
		if (mapDig_[vec.y][vec.x] > 0)						// mapDig_の値が０より大きければremoveしなくていいのでfalse
		{
			return false;
		}
		return true;
		}
	);														// remove_ifのカッコ内 ... 序実関数

	// 別解１(変数に一度入れる)
	//----------------------------
	//auto prg = [&](Vector2 vec) {				// ラムダ式 ... []() {}; -> [] ... キャプチャー部、() ... 引数部、{} ... 実体部(処理の中身)		型推論(auto)で記述が楽		remove_ifは引数が１つ必要
	//	if (mapDig_[vec.y][vec.x] > 0)			// mapDig_の値が０より大きければremoveしなくていいのでfalse
	//	{
	//		return false;
	//	}
	//	return true;
	//};
	//digPosList_.remove_if(prg);
	// 別解２(mapDig_[pos.y][pos.x]--も含めたremove_if)
	//----------------------------------------------------
	//digPosList_.remove_if([&](Vector2 vec) {
	//	return !(--mapDig_[vec.y][vec.x]);
	//	}
	//);
}


bool MapCtl::EraceGold(Vector2 pos, bool playerFlag)
{
	if (GetMapData(ChipGP::ITEM, pos, false) == ChipID::GOLD)					// そのマスに金塊があるかを判断
	{
		if (playerFlag)													// 自機が金塊を取得したら金塊の残り数を減らす
		{
			goldCnt_--;
		}
		SetMapData(pos, ChipID::BLANK, ChipGP::ITEM);			// 金塊があった場所をBLANKに差し替える
	}
	return false;
}


void MapCtl::InputButtonL(bool input)
{
	if (input)					// 押されたら“青色”
	{
		CrL_ = 0x0000ff;
	}
	if (!input)					// 初期状態は“黄色”
	{
		CrL_ = 0xffff00;
	}
}


void MapCtl::InputButtonR(bool input)
{
	if (input)					// 押されたら“青色”
	{
		CrR_ = 0x0000ff;
	}
	if (!input)					// 初期状態は“黄色”
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
	std::ofstream ofp("mapdata.txt", std::ios::binary);			// 書き込みテキスト
	ofp.write(reinterpret_cast<char*>(&expData), sizeof(expData));
	ofp.write(reinterpret_cast<char*>(baseMapID_.data()), baseMapID_.size() * sizeof(baseMapID_[0]));
	// 別解
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
	InitMap();			// 先にデータをクリアする

	DataHeader expData;

	std::ifstream ifp(fileName, std::ios::binary);			// 読み出しテキスト
	ifp.read(reinterpret_cast<char*>(&expData), sizeof(expData));
	ifp.read(reinterpret_cast<char*>(baseMapID_.data()), baseMapID_.size() * sizeof(baseMapID_[0]));

	bool headerFlag = true;
	if (std::string(expData.fileID) != LDR_ID_NAME)			// IDの確認処理
	{
		headerFlag = false;
		TRACE("データがありません\n");
	}

	if (expData.verID != LDR_ID_VER)				// バージョンの確認処理
	{
		headerFlag = false;
		TRACE("バージョンが違います\n");
	}

	unsigned char sum = 0;
	for (auto num : baseMapID_)
	{
		sum += static_cast<unsigned char>(num);
		if (GetMapDataEdit(ChipGP::ITEM, num) == ChipID::GOLD)		// マップ内の金塊の数を計測
		{
			goldCnt_++;
		}
	}

	if (expData.sum != sum)						// sum値の確認処理
	{
		headerFlag = false;
		TRACE("sum値が異常です\n");
	}

	if (!headerFlag)				// ID、バージョン番号、SUM値いずれかが正しく読み込まれていなかったらfalseをリターン
	{
		InitMap();					// データをクリアしておく
		return headerFlag;
	}

	// まとめて確認処理する場合
	//---------------------------
	//unsigned char sum = 0;
	//for (auto num : baseMapID_)
	//{
	//	sum += static_cast<unsigned char>(num);
	//}

	//if ((LDR_ID_NAME) && (expData.verID == LDR_ID_VER) && (expData.sum == sum))				// ID、バージョン番号、SUM値が正しく読み込まれているか確認
	//{
	//	InitMap();					// データをクリアしておく
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
				(*itr)->SetPos(Vector2 (x, y) * chipSize_);							// 現在座標セット			ObjList -> ObjList::iterator -> std::shared_ptr<Obj> -> Obj -> SetPos()
				(*itr)->SetDefPos(Vector2(x, y) * chipSize_);						// デフォルト座標セット

				if (!camera_.expired())											// camera_のweak_ptrの先(shared_ptr)が有効かどうか判断
				{
					camera_.lock()->SetTarget(*itr);
				}
			}

			// Enemy
			//--------
			if (GetMapData(ChipGP::CHAR, x, y, false) == ChipID::ENEMY)
			{
				auto itr = scene.AddObjList(std::make_shared<Enemy>(scene.GetSceneID(), camera_));
				(*itr)->SetPos(Vector2(x, y) * chipSize_);							// 現在座標セット			ObjList -> ObjList::iterator -> std::shared_ptr<Obj> -> Obj -> SetPos()
				(*itr)->SetDefPos(Vector2(x, y) * chipSize_);						// デフォルト座標セット
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
	// はしご or 金塊 のアニメーション
	//----------------------------------
	auto ConvID = [&](ChipID id) {
		if ((id == ChipID::LAD) || (id == ChipID::GOLD))
		{
			return (static_cast<unsigned int>(id) + abs(((animCnt_ / 40) % 4) - 2) * (static_cast<unsigned int>(ChipID::MAX)));				// (/ 40) ... フレーム間の時間, (% 4) ... アニメーションのコマ数 - 1, (- 2) ... 2.1.0.1.2.1.0...とコマが進む, (* 11) ... tilesetのX座標
		}
		return static_cast<unsigned int>(id);
	};


	// マップチップ描画
	//-------------------
	Vector2 camOffset;			// 描画用オフセットの変数
	if (!camera_.expired())
	{
		// カメラが生きていれば描画用オフセットをセットする
		camOffset = camera_.lock()->GetDrawOffset();
	}
	int chipOffsetX = -(camOffset.x / chipSize_.x);
	for (auto chipGP : ChipGP())				// ChipGPを範囲for文で回す
	{
		for (int y = 0; y < worldArea_.y; y++)					// 
		{
			for (int x = chipOffsetX; x <= viewArea_.x + chipOffsetX; x++)					// マス目座標系
			{
				auto id = GetMapData(chipGP, x, y, false);				// GetMapDataを呼び出し				trueにするとDigのアニメーションが出ない
				DrawChip_[id](x, y, chipSize_, camOffset, lpImageMng.GetID("tileset")[ConvID(id)]);
				//DrawChipDef()(x, y, chipSize_, camOffset, lpImageMng.GetID("tileset")[id]);
				//DrawGraph(x * chipSize_.x + camOffset.x, y * chipSize_.y + camOffset.y, lpImageMng.GetID("tileset")[id], true);					// 画面にセットするchipIDの表示
			}
		}
	}
}


void MapCtl::DrawEditLine(void)
{
	for (int y = 0; y <= viewArea_.y; y++)							// 編集画面用横ライン（横ラインを縦に伸ばす）
	{
		DrawLine(0, chipSize_.y * y, viewArea_.x * chipSize_.x, chipSize_.y * y, 0xffffff);
	}
	for (int x = 0; x <= viewArea_.x; x++)							// 編集画面用縦ライン（縦ラインを横に伸ばす）
	{
		DrawLine(chipSize_.x * x, 0, chipSize_.x * x, viewArea_.y * chipSize_.y, 0xffffff);
	}
}


MapCtl::MapCtl(SceneID sceneID)
{
	sceneID_ = sceneID;					// sceneIDを保存
	Init();

	if (sceneID_ == SceneID::EDIT)
	{
		DrawFunc = std::bind(&MapCtl::EditDraw, this);					// EditSceneで、マップと線を描画
		GetMapDataFunc = std::bind(&MapCtl::GetMapDataEdit, this, std::placeholders::_1, std::placeholders::_2);				// std::placeholders::_1 ... GetMapDataEditの()の中身の一番目の変数
		InitDrawChipEdit();
	}
	else
	{
		DrawFunc = std::bind(&MapCtl::GameDraw, this);					// GameSceneで、マップのみを描画
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


	chipGP_[ChipID::BLANK]    = ChipGP::MAX;			// 空白
	chipGP_[ChipID::BBL]      = ChipGP::BG;				// 掘れるブロック
	chipGP_[ChipID::BL]       = ChipGP::BG;				// 掘れないブロック
	chipGP_[ChipID::LAD]      = ChipGP::BG;				// はしご
	chipGP_[ChipID::BAR]      = ChipGP::BG;				// バー
	chipGP_[ChipID::TRAP]     = ChipGP::BG;				// トラップ
	chipGP_[ChipID::ESC]      = ChipGP::BG;				// 脱出はしご
	chipGP_[ChipID::GOLD]     = ChipGP::ITEM;			// ゴールド
	chipGP_[ChipID::ENEMY]    = ChipGP::CHAR;			// 敵
	chipGP_[ChipID::PL]       = ChipGP::CHAR;			// 自機
	chipGP_[ChipID::EDIT_CUR] = ChipGP::CHAR;			// edit用カーソル


	InitMap();

	//EditDraw();

	return true;
}


bool MapCtl::InitMap(void)
{
	baseMapID_.resize(worldArea_.x * worldArea_.y);			// マップの総サイズ数		e縦方向...y
	mapID_.resize(worldArea_.y);							// 横方向...x

	for (int y = 0; y < worldArea_.y; y++)					// for (int y = 0; y < mapID_.size(); y++) でも可
	{
		mapID_[y] = &baseMapID_[worldArea_.x * y];			// mapID_にbaseMapID_のポインタを格納
	}

	for (auto& data : baseMapID_)							// 参照でデータを書き換え
	{
		data = 0;
	}

	InitDig();
	goldCnt_ = 0;

	return true;
}


bool MapCtl::InitDig(void)
{
	baseMapDig_.resize(worldArea_.x * worldArea_.y);		// マップの総サイズ数		縦方向...y
	mapDig_.resize(worldArea_.y);							// 横方向...x

	for (int y = 0; y < worldArea_.y; y++)					// for (int y = 0; y < mapID_.size(); y++) でも可
	{
		mapDig_[y] = &baseMapDig_[worldArea_.x * y];			// mapID_にbaseMapID_のポインタを格納
	}

	for (auto& data : baseMapDig_)							// 参照でデータを書き換え
	{
		data = 0;
	}

	return true;
}


void MapCtl::InitDrawChipEdit()
{
	DrawChip_.try_emplace(ChipID::BLANK,    DrawChipNon());				// 空白				// try_emplase ... 情報が入ってれば何もしないが情報が入っていなければ情報を入れる
	DrawChip_.try_emplace(ChipID::BBL,      DrawChipDef());				// 掘れるブロック
	DrawChip_.try_emplace(ChipID::BL,       DrawChipDef());				// 掘れないブロック
	DrawChip_.try_emplace(ChipID::LAD,      DrawChipDef());				// はしご
	DrawChip_.try_emplace(ChipID::BAR,      DrawChipDef());				// バー（棒）
	DrawChip_.try_emplace(ChipID::TRAP,     DrawChipDef());				// トラップ（落とし穴）
	DrawChip_.try_emplace(ChipID::ESC,      DrawChipDef());				// 脱出はしご
	DrawChip_.try_emplace(ChipID::GOLD,     DrawChipDef());				// ゴールド（金塊）
	DrawChip_.try_emplace(ChipID::ENEMY,    DrawChipDef());				// 敵
	DrawChip_.try_emplace(ChipID::PL,       DrawChipDef());				// 自機
	DrawChip_.try_emplace(ChipID::EDIT_CUR, DrawChipNon());				// edit用カーソル
}


void MapCtl::InitDrawChipGame()
{
	DrawChip_.try_emplace(ChipID::BLANK,    DrawChipNon());				// 空白
	DrawChip_.try_emplace(ChipID::BBL,      DrawChipBbl());				// 掘れるブロック
	DrawChip_.try_emplace(ChipID::BL,       DrawChipDef());				// 掘れないブロック
	DrawChip_.try_emplace(ChipID::LAD,      DrawChipDef());				// はしご
	DrawChip_.try_emplace(ChipID::BAR,      DrawChipDef());				// バー（棒）
	DrawChip_.try_emplace(ChipID::TRAP,     DrawChipTrap());			// トラップ（落とし穴）
	DrawChip_.try_emplace(ChipID::ESC,      DrawChipDef());				// 脱出はしご
	DrawChip_.try_emplace(ChipID::GOLD,     DrawChipDef());				// ゴールド（金塊）
	DrawChip_.try_emplace(ChipID::ENEMY,    DrawChipNon());				// 敵
	DrawChip_.try_emplace(ChipID::PL,       DrawChipNon());				// 自機
	DrawChip_.try_emplace(ChipID::EDIT_CUR, DrawChipNon());				// edit用カーソル
}
