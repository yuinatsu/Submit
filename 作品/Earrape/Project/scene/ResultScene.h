#pragma once
#include<random>
#include "BaseScene.h"
#include"GameScene.h"

constexpr int FontOffsetX = 300;			//空白部分Ｘ
constexpr int FontOffsetY = 60;				//空白部分Ｙ
constexpr int RankResult_SizeX = 160;		//ランクの画像サイズＸ
constexpr int RankResult_SizeY = 80;		//ランクの画像サイズＹ
constexpr int RankResult_Cnt = 3;			//ランクの数
constexpr int SpeechBalloon_SizeX = 530;	//吹き出しの画像サイズＸ
constexpr int SpeechBalloon_SizeY = 180;	//吹き出しの画像サイズＹ
constexpr int DrumLimit = 180;				//ドラムロールが鳴る時間（仮）
constexpr int EarCommentCnt = 21;			//耳のコメントの数

class ResultScene :
    public BaseScene
{
public:
	ResultScene();
	~ResultScene();

	SCN_ID Update(char* keyData, char* keyDataOld) override;
	void Draw(void) override;		// グラフィックの描画
	bool Init(void) override;		// 初期化
	bool Release(void) override;	// リソースの解放

private:

	int DrumCount_;				//ドラムロールのカウント 
	void DrawResult(void);		//スコアとランクの描画 

	//ハンドル集
	int BackGroundImage_;		//背景のハンドル
	//int SpeechBalloonImage_;	//吹き出しのハンドル

	//フォント画像集
	int ScoreImage_;			//SCOREのハンドル
	int RankImage_;				//RANKのハンドル
	int ABCsImage_[3];			//ABCのハンドル

	int EarComment_[EarCommentCnt + 1];			//耳のコメントハンドル
	int CommentNum_;

	std::random_device rnd;		//乱数
	int DrumrollSE_;
	int roll_SE_;

	bool rollSEflag_;
};

