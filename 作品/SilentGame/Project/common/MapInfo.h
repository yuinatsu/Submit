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

	// �}�b�v�̑傫��
	MapSize size_;

	// �G�̐�
	int enemyNum_;

	// �J�����̐�
	int securityCamNum_;

	// �X�e�[�W�摜�̃}�X��
	int stageImgIdx_;
};
