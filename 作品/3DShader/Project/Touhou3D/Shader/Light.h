#pragma once
#include "../Common/Vector3.h"

// ディレクションライト
struct DirectionLight
{
	Vector3 dir;			// 放射方向
	float padding1;			// 詰め物
	Vector3 color;			// RGBカラー
	float padding2;			// 詰め物
	Vector3 eyePos;			// 視点座標
};
// ポイントライト
struct PointLight
{
	Vector3 pos;			// 座標
	float padding1;			// 詰め物
	Vector3 color;			// RGBカラー
	float padding2;			// 詰め物
	float inflRange;		// 影響範囲(m)
};
// スポットライト
struct SpotLight
{
	Vector3 pos;			// 座標
	float padding1;			// 詰め物
	Vector3 color;			// RGBカラー
	float padding2;			// 詰め物
	Vector3 dir;			// 放射方向
	float padding3;			// 詰め物
	float angle;			// 放射角度
	float inflRange;		// 影響範囲(m)
};