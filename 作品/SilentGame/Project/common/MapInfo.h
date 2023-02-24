#pragma once
#include "MapSize.h"

struct MapInfo
{
	constexpr MapInfo(): 
	size_{MapSize::Small}, enemyNum_{0}, securityCamNum_{0}, stageImgIdx_{0}
	{
	}
	constexpr MapInfo(const MapInfo& mapInfo) :
		size_{mapInfo.size_}, enemyNum_{mapInfo.enemyNum_}, securityCamNum_{mapInfo.securityCamNum_}, stageImgIdx_{ mapInfo.stageImgIdx_}
	{
	}
	constexpr MapInfo& operator=(MapInfo& mapInfo)
	{
		size_ = mapInfo.size_;
		enemyNum_ = mapInfo.enemyNum_;
		securityCamNum_ = mapInfo.securityCamNum_;
		return *this;
	}

	// マップの大きさ
	MapSize size_;

	// 敵の数
	int enemyNum_;

	// カメラの数
	int securityCamNum_;

	// ステージ画像のマス目
	int stageImgIdx_;
};
