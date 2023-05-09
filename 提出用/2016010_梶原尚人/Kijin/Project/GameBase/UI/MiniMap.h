#pragma once
#include <map>
#include <DxLib.h>
#include "UiBase.h"
#include "../Common/Math.h"
#include "../Object/ObjectManager.h"

//	�G�Ƃ̋����Ɗp�x���i�[
struct MiniMapRadar
{
	float angle;
	float range;
	float flag;
};

class MiniMap :
	public UiBase
{
	// �~�j�}�b�v�ɓG��\������͈�
	static constexpr float RADAR_RANGE = 90;
public:
	MiniMap(const std::filesystem::path& path);


	void Update(float delta, BaseScene& scene, ObjectManager& objMng, Controller& controller) final;
	void Draw(int mainScr) final;
private:
	UiID GetUIID(void) final { return UiID::MinMap; }
	// �v���C���[�ƃG�l�~�[�̋����Ɗp�x���G�l�~�[�̐������擾
	std::map<int, MiniMapRadar> miniMap_;

	// map�ɂǂꂾ���i�[���������擾
	int count_;

	// �~�j�}�b�v�̃T�C�Y
	int miniMapSize_;
	// �`�悷��X�N���[���̍쐬
	int MapScr_;
	// �摜�̕`��
	int MapGraph_;

};

