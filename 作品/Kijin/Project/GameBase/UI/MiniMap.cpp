#include "MiniMap.h"
#include "../Component/Transform/Transform.h"
#include "../Component/Behavior/StageBehavior.h"
MiniMap::MiniMap(const std::filesystem::path& path) : UiBase{zeroVector2<float>}
{
	count_ = 0;
	int x, y;
	GetDrawScreenSize(&x, &y);
	miniMapSize_ = y / 4;
	MapScr_ = MakeScreen(miniMapSize_, miniMapSize_);
	MapGraph_ = LoadGraph(path.c_str());
}

void MiniMap::Update(float delta, BaseScene& scene, ObjectManager& objMng, Controller& controller)
{
	// �v���C���[�̍��W���擾
	auto player = objMng.GetComponent<Transform>(objMng.GetPlayerID());
	auto& pos = player->GetPos();
	auto pPos_ = Vector2(pos.x, pos.z);
	// �G�l�~�[�̏����擾
	auto stage = objMng.GetComponent<StageBehavior>(objMng.GetStageID());
	auto& list = stage->GetEnemyPosList();
	int i = 0;
	for (auto& enemy : list)
	{
		MiniMapRadar miniMap;
		// �v���C���[�Ƃ̋������擾
		Vector2 tmp = enemy.second - pPos_;
		miniMap.range = tmp.Magnitude();
		// �J�����̏����擾
		auto camera = objMng.GetComponent<Transform>(objMng.GetCameraID());
		// �J�����̌����Ă����������̊p�x�����߂�
		Vector2 camDir = Vector2(camera->GetForward().x, camera->GetForward().z);
		// �p�x�����߂�
		miniMap.angle = GetAngle2Vector(camDir, tmp);
		// �f�t�H���g�͕\�����Ȃ�
		miniMap.flag = false;
		// �������\���\�͈͓��ł���Ε\������
		if (miniMap.range <= (RADAR_RANGE * RADAR_RANGE))
		{
			miniMap.flag = true;
		}
		// �G���ǉ��ŏo�������ꍇ�͒ǉ�����(����̓`���[�g���A���̂�)
		if (miniMap_.size() < list.size())
		{
			miniMap_.emplace(i, miniMap);
			count_++;
		}
		else
		{
			miniMap_.at(i) = miniMap;
		}
		i++;
	}
	// �G�����S���邲�Ƃ�map�̗v�f����������
	if (i < count_)
	{
		count_--;
		miniMap_.erase(count_);
	}
}

void MiniMap::Draw(int mainScr)
{
	// ���[�_�[�̂悤�ȃ~�j�}�b�v���쐬
	SetDrawScreen(MapScr_);
	ClsDrawScreen();
	DrawGraph(0, 0, MapGraph_, true);

	// ���S�ʒu�̎擾
	int SizeHalf = miniMapSize_ / 2;
	// �v���C���[�̈ʒu
	DrawCircle(SizeHalf, SizeHalf, 5, 0x0000ff);

	for (const auto& map : miniMap_)
	{
		if (map.second.flag)
		{
			Vector2 pos = Vector2(0, -1);
			Vector2 tmp;
			// �󂯎�����p�x��sin��cos�̒l������
			float sinParam = sin(-map.second.angle);
			float cosParam = cos(-map.second.angle);
			// sinParam��cosParam�œG�̈ʒu���擾����
			tmp.x = (pos.x * cosParam) - (pos.y * sinParam);
			tmp.y = (pos.x * sinParam) + (pos.y * cosParam);
			// �\���ʒu��␳����(0,0�̈ʒu����ɂȂ�̂Œ��S����̈ʒu�ɂ���)
			pos.x = tmp.x * (map.second.range / 50) + SizeHalf;
			pos.y = tmp.y * (map.second.range / 50) + SizeHalf;
			DrawCircle(static_cast<int>(pos.x), static_cast<int>(pos.y), 2, 0xff0000);
		}
	}
	SetDrawScreen(mainScr);
	// �쐬�����~�j�}�b�v�����C���X�N���[���ɕ`��
	DrawGraph(10, 10, MapScr_, true);
}
