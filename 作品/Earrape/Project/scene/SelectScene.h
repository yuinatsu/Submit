#pragma once
#include "BaseScene.h"
#include "../common/Vector2.h"
#include"../common/Geometry.h"

constexpr float LeftEarPos_X = 110.0f;		//最終的な左の耳の位置X
constexpr float CenterEarPos_X = 315.0f;	//最終的な真ん中の耳の位置X
constexpr float RightEarPos_X = 520.0f;		//最終的な右の耳の位置X

constexpr float LREarPos_Y = 15.0f;			//最終的な左右の耳の位置Y
constexpr float CenterEarPos_Y = 90.0f;		//最終的な真ん中の耳の位置Y

constexpr float EarSpeed = 3.0f;			//耳が移動するときの速度 
constexpr float EarAccel = 0.5f;			//耳の加速度

constexpr int ArrowSizeX = 80;				//矢印のサイズX
constexpr int ArrowSizeY = 35;			//左矢印のサイズY

constexpr int StartSizeX = 120;				//スタートのサイズX
constexpr int StartSizeY = 54;				//スタートのサイズY


struct EarData
{
	Position2 pos;
	Position2 anotherpos;
	Vector2_2 vel;
	float Accel_ = 0.0f;
	bool MoveFlagR = false;
	bool MoveFlagL = false;
};

class SelectScene :
	public BaseScene
{
public:
	SelectScene() {
		selectbg_ = selectLogoBO_ = selectLogoBW_ = 0;
		blinkCnt_ = 0;
	};
	~SelectScene() {};
	SCN_ID Update(char* keyData, char* keyDataOld) override;
	void EarMoveR(void);		//右方向への耳の移動
	void EarMoveL(void);		//左方向への耳の移動

	void Draw(void) override;	// グラフィックの描画
	bool Init(void) override;	// 初期化
	bool Release(void) override;	// リソースの解放
private:
	int pageCnt_;			// ページカウンター
	Vector2 pos_;


	EarData LeftEar;	//現在の右の耳の位置
	EarData CenterEar;	//現在の真ん中の耳の位置
	EarData RightEar;	//現在の左の耳の位置
	EarData AddEar;		//追加される耳

	const Vector2 RUear = { 840 ,-EarSizeY };
	const Vector2 LUear = { -EarSizeX,-EarSizeY };
	int selectbg_;
	int selectLogoBO_;
	int selectLogoBW_;


	int blinkCnt_;

	//ハンドル
	int RightArrow_;
	int LeftArrow_;
	int StartButton_;

	int Select_SE_;
	int atfSE_;

	bool ImageChangeF_;
	bool Lflag_;
	bool Rflag_;
};

