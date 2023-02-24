#pragma once
#include <list>
#include <string>
#include<array>
#include "Math.h"
#include "MapCollMng.h"

//using namespace Math;

struct Traiangle
{
	Math::Vector2 pos1;
	Math::Vector2 pos2;
	Math::Vector2 pos3;
};

struct Squar
{
	Math::Vector2 sPos;			// 左上の座標
	Math::Vector2 ePos;			// 右上の座標
	bool flag_;				// 確認用
	Squar() :sPos(0.0f, 0.0f), ePos(0.0f, 0.0f),flag_(false) {};
	Squar(const Math::Vector2& spos, const Math::Vector2& epos);
	void Update(const Math::Vector2& pos);
	void Draw(const Math::Vector2& offsetpos);
	bool Getflag(void);
	Math::Vector2 GetsPos(void);
};

struct Rect
{
	Math::Vector2 r1;
	Math::Vector2 r2;
	Math::Vector2 r3;
	Math::Vector2 r4;
	Rect() : r1(0, 0), r2(0, 0), r3(0, 0), r4(0, 0) {};
	Rect(const Math::Vector2& rr1, const Math::Vector2& rr2, const Math::Vector2& rr3, const Math::Vector2& rr4);
};

using Rectlist = std::list<Rect>;

// 描画とプレイヤーとの当たり判定は他の方のプログラムを参考にしました。
// それ以外は使いやすいようにこちらで改良しました
struct Fan
{
	Math::Vector2 center;				// 中心座標
	Math::Vector2 v1;					// 扇形の端ベクトル
	Math::Vector2 v2;					// 扇形の端ベクトル
	Math::Vector2 vv1;				// 扇形の端ベクトル(角度を求めるためにv1を避難させる場所)
	Math::Vector2 vv2;				// 扇形の端ベクトル()
	Math::Vector2 vf1;				// 壁に当たっているときの扇形の端ベクトル
	Math::Vector2 vf2;				// 壁に当たっているときの扇形の端ベクトル

	Rectlist reclist_;
	bool hitFlag;				// プレイヤーと当たっているか
	Fan() :center(0, 0), v1(0, 0), v2(0, 0), vv1(0, 0), vv2(0, 0), vf1(0, 0), vf2(0, 0), hitFlag(false) {};
	Fan(const Math::Vector2& p, const Math::Vector2& inv1, const Math::Vector2& inv2);
	Fan(const Math::Vector2& p, float r, float angle);
	Fan(const Math::Vector2& p, const Math::Vector2& inv, float angle, ColData col1, ColData col2);
	void Checkhit(const Math::Vector2& pos);
	// 点と四角の当たり判定
	bool UpdateHitRect(const Math::Vector2& pos);
	bool UpdateHitPos(const Math::Vector2& pos);
	void Draw(const Math::Vector2& offsetpos);
	float Radius(void) const;
	//２つの座標から角度を求める
	float GetAngle(void)const;
	float GetAngleV2(void)const;
	void AddAngle(float angle);
	// 視線にプレイヤーが入っているかを渡す
	bool GetFlag(void);
};

float GetAngle2Vector(const Math::Vector2& v1, const Math::Vector2& v2);
// 線分同士の当たり判定
bool CheckhitLine(Math::Vector2 a1, Math::Vector2 a2, Math::Vector2 b1, Math::Vector2 b2);
// 矩形と線分の当たり判定
//(プレイヤーと矩形どっちが監視カメラの中心点に近いかで当たり判定をするかどうか決める)
bool RecthitLine(Rect rect, Math::Vector2 b1, Math::Vector2 b2,Math::Vector2 pos);


